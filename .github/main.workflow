workflow "Build and Deploy" {
  on = "push"
  resolves = [
    "Verify GKE deployment",
  ] 
}

# Build

action "Build Docker image" {
  uses = "actions/docker/cli@master"
  args = ["build", "-t", "reasonml-postgres", "."]
}

# Deploy Filter

action "Deploy branch filter" {
  needs = ["Set Credential Helper for Docker"]
  uses = "actions/bin/filter@master"
  args = "branch master"
}

# GKE

action "Setup Google Cloud" {
  uses = "actions/gcloud/auth@master"
  secrets = ["GCLOUD_AUTH"]
}

action "Tag image for GCR" {
  needs = ["Setup Google Cloud", "Build Docker image"]
  uses = "actions/docker/tag@master"
  env = {
    PROJECT_ID = "radiant-destiny-219301"
    APPLICATION_NAME = "reasonml-postgres"
  }
  args = ["reasonml-postgres", "gcr.io/$PROJECT_ID/$APPLICATION_NAME"]
}

action "Set Credential Helper for Docker" {
  needs = ["Setup Google Cloud", "Tag image for GCR"]
  uses = "actions/gcloud/cli@master"
  args = ["auth", "configure-docker", "--quiet"]
}

action "Push image to GCR" {
  needs = ["Setup Google Cloud", "Deploy branch filter"]
  uses = "actions/gcloud/cli@master"
  runs = "sh -c"
  env = {
    PROJECT_ID = "radiant-destiny-219301"
    APPLICATION_NAME = "reasonml-postgres"
  }
  args = ["docker push gcr.io/$PROJECT_ID/$APPLICATION_NAME"]
}

action "Load GKE kube credentials" {
  needs = ["Setup Google Cloud", "Push image to GCR"]
  uses = "actions/gcloud/cli@master"
  env = {
    PROJECT_ID = "radiant-destiny-219301"
    CLUSTER_NAME = "batman"
  }
  args = "container clusters get-credentials $CLUSTER_NAME --zone us-central1-a --project $PROJECT_ID"
}

# TODO Add Action to start GitHub Deploy

action "Deploy to GKE" {
  needs = ["Push image to GCR", "Load GKE kube credentials"]
  uses = "docker://gcr.io/cloud-builders/kubectl"
  env = {
    PROJECT_ID = "radiant-destiny-219301"
    APPLICATION_NAME = "reasonml-postgres"
    DEPLOYMENT_NAME = "reasonml-postgres"
  }
  runs = "sh -l -c"
  args = ["SHORT_REF=$(echo ${GITHUB_SHA} | head -c7) && cat $GITHUB_WORKSPACE/reasonml-postgres-deployment.yaml | sed 's/PROJECT_ID/'\"$PROJECT_ID\"'/' | sed 's/APPLICATION_NAME/'\"$APPLICATION_NAME\"'/' | sed 's/TAG/'\"$SHORT_REF\"'/' | kubectl apply -f - "]
}

action "Verify GKE deployment" {
  needs = ["Deploy to GKE"]
  uses = "docker://gcr.io/cloud-builders/kubectl"
  env = {
    DEPLOYMENT_NAME = "reasonml-postgres"
  }
  args = "rollout status deployment/reasonml-postgres"
}
