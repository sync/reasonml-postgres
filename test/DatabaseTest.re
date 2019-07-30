open TestFramework;
open Library.Database;

let {describe, describeSkip, describeOnly} =
  describeConfig
  |> withLifecycle(testLifecycle =>
       testLifecycle
       |> beforeEach(Utils.Database.reset)
       |> afterAll(Utils.Database.reset)
     )
  |> build;

describe("Database", ({describe, test, _}) => {
  describe("rollback", ({test, _}) => {
    test("should drop the table", ({expect}) => {
      let result = rollback() |> Lwt_main.run;
      expect.result(result).toBeOk();
    })
  })
});
