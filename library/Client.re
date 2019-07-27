open Lwt.Infix;

module Json = {
  include Yojson.Basic.Util;

  let member_opt = (m, t) =>
    try(Some(member(m, t))) {
    | _ => None
    };
};

let execute_query = (~query) => {
  let url = Uri.of_string("http://localhost:8080/graphql");

  let queryJson =
    `Assoc([
      ("query", `String(query#query)),
      ("variables", query#variables),
    ]);
  let reqBody = Yojson.Safe.to_string(queryJson);

  Logs.debug(m => m("reqBody: %s", reqBody));

  let body = Cohttp_lwt.Body.of_string(reqBody);

  let baseHeaders =
    Cohttp.Header.init_with("Content-Type", "application/json");
  let headers =
    Cohttp.Header.add(
      baseHeaders,
      "content-length",
      string_of_int(String.length(reqBody)),
    );

  Cohttp_lwt_unix.Client.post(url, ~body, ~headers)
  >>= (
    ((_resp, body)) =>
      Cohttp_lwt.Body.to_string(body)
      >|= (
        body => {
          Logs.debug(m => m("got body: %s", body));

          // TODO: catch error from either from_string or query parse
          let json_result = Yojson.Basic.from_string(body);
          switch (Json.member_opt("data", json_result)) {
          | Some(data) => Ok(query#parse(data))
          | None => Error("Didn't get anything")
          };
        }
      )
  );
};

let get_subreddit = (~name) => {
  Logs.debug(m => m("Getting subreddit: %s", name));

  module SubredditQuery = [%graphql
    {|
      query Subreddit($name: String!) {
        subreddit(name: $name) {
          posts {
            id
          }
        }
      }
    |}
  ];

  let query = SubredditQuery.make(~name, ());
  execute_query(~query);
};

let get_links = () => {
  Logs.debug(m => m("Getting links"));

  module LinksQuery = [%graphql
    {|
      query Links {
        links {
          id
        }
      }
    |}
  ];

  let query = LinksQuery.make();
  execute_query(~query);
};
