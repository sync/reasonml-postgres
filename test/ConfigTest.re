open TestFramework;
open Library.Config;

describe("Environment", ({test, _}) => {
  test("by default", ({expect}) => {
    let environment = Environment.values;

    expect.string(environment.db_connection).not.toBeEmpty();
  });

  test("with a connection", ({expect}) => {
    let fake_connection = "test-fake";
    let environment = Factory.Environment.make(None, Some(fake_connection));

    expect.string(environment.db_connection).toEqual(fake_connection);
  });

  test("with a test env", ({expect}) => {
    let environment = Factory.Environment.make(Some("TEST"), None);

    expect.string(environment.db_connection).toEqual(
      "postgresql://postgres:postgres@localhost:5432/community_test",
    );
  });

  test("with a test env and a connection", ({expect}) => {
    let fake_connection = "test-fake";
    let environment =
      Factory.Environment.make(Some("TEST"), Some(fake_connection));

    expect.string(environment.db_connection).toEqual(fake_connection);
  });

  test("without a test env and a connection", ({expect}) => {
    let environment = Factory.Environment.make(None, None);

    expect.string(environment.db_connection).toEqual(
      "postgresql://postgres:postgres@localhost:5432/community_dev",
    );
  });
});
