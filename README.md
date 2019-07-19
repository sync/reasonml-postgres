# reasonml-postgres

**Contains the following libraries and executables:**

```
reasonml-postgres@0.0.0
│
├─test/
│   name:    TestReasonmlPostgres.exe
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
