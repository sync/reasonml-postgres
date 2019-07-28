open TestFramework;
open Library.Environment;

describe("Environment", ({test, _}) => {
  test("by default", ({expect}) => {
    let environment = CurrentEnvironment.values;

    expect.string(environment.db_connection).toEqual(
      "postgresql://postgres:postgres@localhost:5432/community_test",
    );
  });

  test("with a connection", ({expect}) => {
    let fake_connection = "test-fake";

    module TestEnvProvider: EnvironmentProvider = {
      let env = None;
      let db_connection = Some(fake_connection);
    };

    module TestEnvironment = Make(TestEnvProvider);
    let environment = TestEnvironment.values;

    expect.string(environment.db_connection).toEqual(fake_connection);
  });

  test("with a test env and a connection", ({expect}) => {
    let fake_connection = "test-fake";

    module TestEnvProvider: EnvironmentProvider = {
      let env = Some("TEST");
      let db_connection = Some(fake_connection);
    };

    module TestEnvironment = Make(TestEnvProvider);
    let environment = TestEnvironment.values;

    expect.string(environment.db_connection).toEqual(fake_connection);
  });

  test("without a test env and a connection", ({expect}) => {
    module TestEnvProvider: EnvironmentProvider = {
      let env = None;
      let db_connection = None;
    };

    module TestEnvironment = Make(TestEnvProvider);
    let environment = TestEnvironment.values;

    expect.string(environment.db_connection).toEqual(
      "postgresql://postgres:postgres@localhost:5432/community_dev",
    );
  });
});
