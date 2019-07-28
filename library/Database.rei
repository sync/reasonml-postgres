module Link: {
  type t = {
    id: int,
    url: string,
  };
};

let migrate: unit => Lwt.t(result(unit, string));
let rollback: unit => Lwt.t(result(unit, string));
let seed:
  (~urls: list(string)=?, unit) => Lwt.t(list(Stdlib.result(unit, string)));

let get_all_links: unit => Lwt.t(result(list(Link.t), string));

let get_tablenames: unit => Lwt.t(result(list(string), string));
