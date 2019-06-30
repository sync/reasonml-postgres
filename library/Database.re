type link = {
  id: int,
  url: string,
};

type error =
  | Database_error(string);

let error_to_string = error =>
  switch (error) {
  | Database_error(string) => string
  };

/* Helper method to map Caqti errors to our own error type.
   val or_error : ('a, [> Caqti_error.t ]) result Lwt.t -> ('a, error) result Lwt.t */
let or_error = m =>
  switch%lwt (m) {
  | Ok(a) => Ok(a) |> Lwt.return
  | Error(e) => Error(Database_error(Caqti_error.show(e))) |> Lwt.return
  };

let connection_url =
  switch (Sys.getenv_opt("DB_CONNECTION")) {
  | Some(url) => url
  | None => "postgresql://localhost:5432/community_dev"
  };

/* This is the connection pool we will use for executing DB operations. */
let pool =
  switch (
    Caqti_lwt.connect_pool(~max_size=10, Uri.of_string(connection_url))
  ) {
  | Ok(pool) => pool
  | Error(err) => failwith(Caqti_error.show(err))
  };

let get_all_links = () => {
  let get_all_query =
    Caqti_request.collect(
      Caqti_type.unit,
      Caqti_type.(tup2(int, string)),
      "SELECT id, url FROM links",
    );

  let get_all = (module C: Caqti_lwt.CONNECTION) =>
    C.fold(
      get_all_query,
      ((id, url), acc) => [{id, url}, ...acc],
      (),
      [],
    );

  let%lwt result = Caqti_lwt.Pool.use(get_all, pool) |> or_error;

  (
    switch (result) {
    | Ok(links) => Ok(links)
    | Error(err) => Error(error_to_string(err))
    }
  )
  |> Lwt.return;
};
