type environment = {db_connection: string};

module type EnvironmentProvider = {
  let env: option(string);
  let db_connection: option(string);
};

module Make: (E: EnvironmentProvider) => {let values: environment;};

module CurrentEnvironment: {let values: environment;};
