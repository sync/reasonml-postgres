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

/* Helper method to map Caqti errors to our own error type.
   val or_error : ('a, [> Caqti_error.t ]) result Lwt.t -> ('a, error) result Lwt.t */
let or_error = m =>
  switch%lwt (m) {
  | Ok(a) => Ok(a) |> Lwt.return
  | Error(e) => Error(Caqti_error.show(e)) |> Lwt.return
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

// Migrations

module Migrations = {
  let migrate_query =
    Caqti_request.exec(
      Caqti_type.unit,
      {| CREATE TABLE links (
          id BIGSERIAL PRIMARY KEY,
          url character varying(255)
       )
    |},
    );

  let rollback_query =
    Caqti_request.exec(Caqti_type.unit, "DROP TABLE links");
};

let migrate = () => {
  let migrate' = (module Db: Caqti_lwt.CONNECTION) =>
    Db.exec(Migrations.migrate_query, ());

  Caqti_lwt.Pool.use(migrate', pool) |> or_error;
};

let rollback = () => {
  let rollback' = (module Db: Caqti_lwt.CONNECTION) =>
    Db.exec(Migrations.rollback_query, ());

  Caqti_lwt.Pool.use(rollback', pool) |> or_error;
};

// Queries

module Queries = {
  let add_link_query =
    Caqti_request.exec(
      Caqti_type.string,
      "INSERT INTO links (url) VALUES (?)",
    );

  let get_all_links =
    Caqti_request.collect(
      Caqti_type.unit,
      Link.t,
      "SELECT id, url FROM links",
    );
};

let add_url = content => {
  let add' = (content, module Db: Caqti_lwt.CONNECTION) =>
    Db.exec(Queries.add_link_query, content);

  Caqti_lwt.Pool.use(add'(content), pool) |> or_error;
};

let seed = () => {
  Lwt_list.map_p(add_url, ["https://www.test.com", "https://www.apple.com"]);
};

let get_all = query => {
  let get_all = (module Db: Caqti_lwt.CONNECTION) =>
    Db.fold(query, (row, acc) => [row, ...acc], (), []);

  Caqti_lwt.Pool.use(get_all, pool) |> or_error;
};

let get_all_links = () => get_all(Queries.get_all_links);
