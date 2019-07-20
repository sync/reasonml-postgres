let setup = (~level=Logs.Info, ()) => {
  Fmt_tty.setup_std_outputs();
  Logs.set_level(Some(level));
  Logs.set_reporter(Logs_fmt.reporter());
};
