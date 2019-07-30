open TestFramework;
open Library.Database;
open Lwt.Infix;

let {describe, describeSkip, describeOnly} =
  describeConfig
  |> withLifecycle(testLifecycle =>
       testLifecycle
       |> beforeEach(() =>
            rollback() >>= (_ => migrate()) >>= (_ => seed()) |> Lwt_main.run
          )
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
