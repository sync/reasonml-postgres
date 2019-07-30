open Lwt.Infix;

module Database = {
  open Library.Database;

  let reset = () =>
    rollback() >>= (_ => migrate()) >>= (_ => seed()) |> Lwt_main.run |> ignore;
};
