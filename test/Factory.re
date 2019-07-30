module Environment = {
  open Library.Config;
  let make = (env: option(string), db_connection: option(string)) => {
    module TestEnvProvider: EnvironmentProvider = {
      let env = env;
      let db_connection = db_connection;
    };

    module TestEnvironment = Make(TestEnvProvider);
    TestEnvironment.values;
  };
};
