type values = {db_connection: string};

module type EnvironmentProvider = {
  let env: option(string);
  let db_connection: option(string);
};

module type Environment = {let values: values;};

module Make: (E: EnvironmentProvider) => Environment;

module Environment: {let values: values;};
