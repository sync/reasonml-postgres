module Link = {
  type t = {
    id: int,
    url: string,
  };
  let t = {
    let encode = ({id, url}) => Ok((id, url));
    let decode = ((id, url)) => Ok({id, url});
    let rep = Caqti_type.(tup2(int, string));
    Caqti_type.custom(~encode, ~decode, rep);
  };
};

module Q = {
  let get_all_links =
    Caqti_request.collect(
      Caqti_type.unit,
      Link.t,
      "SELECT id, url FROM links",
    );
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
  | None => "postgresql://postgres:postgres@localhost:5432/community_dev"
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
  let get_all = (module Db: Caqti_lwt.CONNECTION) =>
    Db.fold(Q.get_all_links, (link, acc) => [link, ...acc], (), []);

  let%lwt result = Caqti_lwt.Pool.use(get_all, pool) |> or_error;

  (
    switch (result) {
    | Ok(links) => Ok(links)
    | Error(err) => Error(error_to_string(err))
    }
  )
  |> Lwt.return;
};
