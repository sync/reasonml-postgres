// reddit

open Reddit;

let post: Graphql_lwt.Schema.typ(unit, option(post_data)) =
  Graphql_lwt.Schema.(
    obj("Post", ~doc="A subreddit post", ~fields=_ =>
      [
        field(
          "id",
          ~doc="Unique post identifier",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p: post_data) =>
          p.data.id
        ),
        field(
          "title",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p: post_data) =>
          p.data.title
        ),
        field(
          "author",
          ~typ=non_null(string),
          ~args=Arg.[],
          ~resolve=(_info, p: post_data) =>
          p.data.author
        ),
        field(
          "ups",
          ~doc="The upvotes a post has received",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(_info, p: post_data) =>
          p.data.ups
        ),
      ]
    )
  );

let subreddit: Graphql_lwt.Schema.typ(unit, option(subreddit)) =
  Graphql_lwt.Schema.(
    obj("Subreddit", ~doc="A subreddit", ~fields=_ =>
      [
        field(
          "posts",
          ~doc="List of posts",
          ~typ=non_null(list(non_null(post))),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.children
        ),
      ]
    )
  );

// postgres

open Database;

let link: Graphql_lwt.Schema.typ(unit, option(Link.t)) =
  Graphql_lwt.Schema.(
    obj("Link", ~doc="A database link", ~fields=_ =>
      [
        field(
          "id",
          ~doc="Unique link identifier",
          ~typ=non_null(int),
          ~args=Arg.[],
          ~resolve=(_info, p) =>
          p.Link.id
        ),
        field(
          "url", ~typ=non_null(string), ~args=Arg.[], ~resolve=(_info, p) =>
          p.Link.url
        ),
      ]
    )
  );

let set_interval = (s, f, destroy) => {
  let rec set_interval_loop = (s, f, n) => {
    let timeout =
      Lwt_timeout.create(s, () =>
        if (n > 0) {
          f();
          set_interval_loop(s, f, n - 1);
        } else {
          destroy();
        }
      );

    Lwt_timeout.start(timeout);
  };

  set_interval_loop(s, f, 5);
};

let users = ["Anthony", "John", "Paul", "Jason"];

let schema =
  Graphql_lwt.Schema.(
    schema(
      [
        io_field(
          "subreddit",
          ~typ=subreddit,
          ~args=Arg.[arg("name", ~typ=non_null(string))],
          ~resolve=(_info, (), name) =>
          Reddit.get_subreddit(~name)
        ),
        io_field(
          "links",
          ~doc="List of database links",
          ~typ=non_null(list(non_null(link))),
          ~args=Arg.[],
          ~resolve=(_info, ()) =>
          Database.get_all_links()
        ),
      ],
      ~subscriptions=[
        subscription_field(
          "subscribe_to_user",
          ~typ=non_null(string),
          ~args=Arg.[arg'("intarg", ~typ=int, ~default=42)],
          ~resolve=(_info, _intarg) => {
            let (user_stream, push_to_user_stream) = Lwt_stream.create();
            let destroy_stream = () => push_to_user_stream(None);
            set_interval(
              2,
              () => {
                let idx = Random.int(List.length(users));
                push_to_user_stream(Some(List.nth(users, idx)));
              },
              destroy_stream,
            );
            Lwt_result.return((user_stream, destroy_stream));
          },
        ),
      ],
    )
  );
