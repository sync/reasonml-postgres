open Lwt.Infix;

module D = Decoders_yojson.Basic.Decode;

type post = {
  id: string,
  title: string,
  author: string,
  ups: int,
};

let post_decoder: D.decoder(post) = (
  D.(
    field("id", string)
    >>= (
      id =>
        field("title", string)
        >>= (
          title =>
            field("author", string)
            >>= (
              author =>
                field("ups", int)
                >>= (ups => succeed({id, title, author, ups}))
            )
        )
    )
  ):
    D.decoder(post)
);

type post_data = {data: post};

let post_data_decoder: D.decoder(post_data) = (
  D.(field("data", post_decoder) >>= (data => succeed({data: data}))):
    D.decoder(post_data)
);

type subreddit = {children: list(post_data)};

let subreddit_decoder: D.decoder(subreddit) = (
  D.(
    field("children", list(post_data_decoder))
    >>= (children => succeed({children: children}))
  ):
    D.decoder(subreddit)
);

type subreddit_data = {data: subreddit};

let subreddit_data_decoder: D.decoder(subreddit_data) = (
  D.(field("data", subreddit_decoder) >>= (data => succeed({data: data}))):
    D.decoder(subreddit_data)
);

let get_subreddit = (~name) => {
  Logs.debug(m => m("Getting subreddit: %s", name));

  let url =
    Uri.of_string(Format.sprintf("https://www.reddit.com/r/%s.json", name));
  Cohttp_lwt_unix.Client.get(url)
  >>= (
    ((_resp, body)) =>
      Cohttp_lwt.Body.to_string(body)
      >|= (
        body => {
          let decoded = D.(decode_string(subreddit_data_decoder, body));
          switch (decoded) {
          | Ok(result) => Ok(Some(result.data))
          | Error(e) => Error(Format.asprintf("%a", D.pp_error, e))
          };
        }
      )
  );
};
