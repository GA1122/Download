  UrlFetcherTest()
      : test_server_(
          net::TestServer::TYPE_HTTPS,
          net::TestServer::kLocalhost,
          FilePath(FILE_PATH_LITERAL("net/data/url_request_unittest"))),
        io_thread_("TestIOThread"),
        file_thread_("TestFileThread") {
  }
