# reasonml-postgres

**Contains the following libraries and executables:**

```
reasonml-postgres@0.0.0
│
├─test/
│   name:    TestReasonmlPostgres.exe
│   require: reasonml-postgres/library
│
├─integration/
│   name:    IntegrationReasonmlPostgres.exe
│   require: reasonml-postgres/library
│
├─library/
│   library name: reasonml-postgres/library
│   require:
│
└─executable/
    name:    ReasonmlPostgresApp.exe
    require: reasonml-postgres/library
```

## Developing:

```
npm install -g esy@next
git clone <this-repo>
esy install
esy build
```

### MacOS: 

```
brew install postgres
```

### Ubuntu:

```
sudo apt-get install libpq-dev libssl-dev pkg-config
```

## If you experience:

`Operation not permitted mkdir ... .graphql_ppx_cache/`

Temporarily add this to the esy section of your package.json.

```
"esy": {
    "buildsInSource": "unsafe"
}
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy start
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```

## Runnin Repl:

```
esy dune utop library
#require "rtop";;
open Library;
#show Schema;
```

## Kubernetes:

## Create Secrets

```bash
kubectl create secret generic postgres --from-literal=db-connection='YOUR_DB_CONNECTION'
```
