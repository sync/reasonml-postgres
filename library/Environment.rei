type environment = {db_connection: string};

let current:
  (~env: option(string)=?, ~db_connection: option(string)=?, unit) =>
  environment;
