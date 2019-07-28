type values = {db_connection: string};

module type EnvironmentProvider = {
  let env: option(string);
  let db_connection: option(string);
};

module type Environment = {let values: values;};

module Make = (E: EnvironmentProvider) : Environment => {
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

  let values =
    switch (E.env, E.db_connection) {
    | (_, Some(connection)) => create_environment(connection)
    | (Some(env), None) => environment_of_string(env)
    | _ => development
    };
};

module SystemEnvProvider: EnvironmentProvider = {
  let env = Sys.getenv_opt("ENV");
  let db_connection = Sys.getenv_opt("DB_CONNECTION");
};

module Environment = Make(SystemEnvProvider);
