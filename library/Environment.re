type environment = {db_connection: string};

let test = {
  db_connection: "postgresql://postgres:postgres@localhost:5432/community_test",
};

let development = {
  db_connection: "postgresql://postgres:postgres@localhost:5432/community_dev",
};

let create_environment = connection => {db_connection: connection};

let environment_of_string = env =>
  switch (env) {
  | "TEST" => test
  | _ => development
  };

let current =
    (
      ~env=Sys.getenv_opt("ENV"),
      ~db_connection=Sys.getenv_opt("DB_CONNECTION"),
      (),
    ) =>
  switch (env, db_connection) {
  | (_, Some(connection)) => create_environment(connection)
  | (Some(env), None) => environment_of_string(env)
  | _ => development
  };
