open TestFramework;

describe("e2e test suite", ({test, _}) => {
  test("get_subreddit should return posts", ({expect}) => {
    let result =
      Library.Client.get_subreddit(~name="reactjs") |> Lwt_main.run;

    expect.result(result).toBeOk();
  });

  test("get_links should return links", ({expect}) => {
    let result = Library.Client.get_links() |> Lwt_main.run;

    expect.result(result).toBeOk();
  });
});
