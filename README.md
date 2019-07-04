# reasonml-postgres

**Contains the following libraries and executables:**

```
reasonml-postgres@0.0.0
│
├─test/
│   library name: reasonml-postgres/test
│   require: reasonml-postgres/library
│
testExe/
│   name:    RunTests.exe
│   require: reasonml-postgres.test
│
├─library/
│   library name: reasonml-postgres/library
│   require:
│
└─executable/
    name:    Server.exe
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

## Kubernetes:

## Create Secrets

```bash
kubectl create secret generic postgres --from-literal=db-connection='YOUR_DB_CONNECTION'
```
