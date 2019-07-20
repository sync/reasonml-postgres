Loggers.setup();

module Graphql_cohttp_lwt =
  Graphql_cohttp.Make(
    Graphql_lwt.Schema,
    Cohttp_lwt_unix.IO,
    Cohttp_lwt.Body,
  );

let start = () => {
  let on_exn =
    fun
    | [@implicit_arity] Unix.Unix_error(error, func, arg) =>
      Logs.warn(m =>
        m(
          "Client connection error %s: %s(%S)",
          Unix.error_message(error),
          func,
          arg,
        )
      )
    | exn => Logs.err(m => m("Unhandled exception: %a", Fmt.exn, exn));

  let callback = Graphql_cohttp_lwt.make_callback(_req => (), Schema.schema);

  let port = 8080;
  let server = Cohttp_lwt_unix.Server.make_response_action(~callback, ());
  let mode = `TCP(`Port(port));

  Logs.info(m => m("Starting server on port: %d", port));
  Cohttp_lwt_unix.Server.create(~on_exn, ~mode, server) |> Lwt_main.run;
};
