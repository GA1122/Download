  bool DoManyCookiesRequest(int num_cookies) {
    TestDelegate d;
    URLRequest r(test_server_.GetURL("set-many-cookies?" +
                                     base::IntToString(num_cookies)),
                                     &d,
                                     &default_context_);

    r.Start();
    EXPECT_TRUE(r.is_pending());

    MessageLoop::current()->Run();

    bool is_success = r.status().is_success();

    if (!is_success) {
      EXPECT_TRUE(r.status().error() == ERR_RESPONSE_HEADERS_TOO_BIG ||
                  r.status().error() == ERR_CONNECTION_ABORTED);
      EXPECT_TRUE(test_server_.Stop());
      EXPECT_TRUE(test_server_.Start());
    }

    return is_success;
  }
