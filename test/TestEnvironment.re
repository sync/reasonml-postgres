open TestFramework;
open Library.Environment;

describe("Environment", ({test, _}) => {
  test("by default", ({expect}) => {
    let environment = current();

    expect.string(environment.db_connection).toEqual(
      "postgresql://postgres:postgres@localhost:5432/community_test",
    );
  });

  test("with a connection", ({expect}) => {
    let fake_connection = "test-fake";
    let environment = current(~db_connection=Some(fake_connection), ());

    expect.string(environment.db_connection).toEqual(fake_connection);
  });

  test("with a test env and a connection", ({expect}) => {
    let fake_connection = "test-fake";
    let environment =
      current(~env=Some("TEST"), ~db_connection=Some(fake_connection), ());

    expect.string(environment.db_connection).toEqual(fake_connection);
  });

  test("without a test env and a connection", ({expect}) => {
    let environment = current(~env=None, ~db_connection=None, ());

    expect.string(environment.db_connection).toEqual(
      "postgresql://postgres:postgres@localhost:5432/community_dev",
    );
  });
});
