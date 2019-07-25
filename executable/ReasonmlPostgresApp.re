open Cmdliner;
open Library;

Loggers.setup();

let command = {
  let doc = "Accepted commands are: 'start'";
  Arg.(
    required
    & pos(~rev=false, 0, some(string), None)
    & info([], ~docv="COMMAND", ~doc)
  );
};

let start = command => {
  switch (command) {
  | "start" => Server.start()
  | _ => Logs.info(m => m("%s is not a valid command", command))
  };
};

let () = {
  let doc = "The beginning of everything";
  let term = Term.(const(start) $ command);
  let info =
    Term.info(
      "ReasonmlPostgresApp",
      ~doc,
      ~version="0.0.0",
      ~exits=Term.default_exits,
    );

  (term, info) |> Term.eval |> Term.exit;
};
