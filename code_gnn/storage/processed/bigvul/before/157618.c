  void RequestPolicy() {
    MockRead data_reads[] = {
        MockRead("HTTP/1.0 200 OK\r\n"),
        MockRead("NEL: {\"report_to\": \"nel\", \"max_age\": 86400}\r\n"),
        MockRead("\r\n"),
        MockRead("hello world"),
        MockRead(SYNCHRONOUS, OK),
    };
    MockWrite data_writes[] = {
        MockWrite("GET / HTTP/1.1\r\n"
                  "Host: www.example.org\r\n"
                  "Connection: keep-alive\r\n\r\n"),
    };

    HttpRequestInfo request;
    request.method = "GET";
    request.url = GURL(url_);
    request.traffic_annotation =
        net::MutableNetworkTrafficAnnotationTag(TRAFFIC_ANNOTATION_FOR_TESTS);

    SSLSocketDataProvider ssl(ASYNC, OK);
    if (request.url.SchemeIsCryptographic()) {
      ssl.ssl_info.cert =
          ImportCertFromFile(GetTestCertsDirectory(), "wildcard.pem");
      ASSERT_TRUE(ssl.ssl_info.cert);
      ssl.ssl_info.cert_status = cert_status_;
      session_deps_.socket_factory->AddSSLSocketDataProvider(&ssl);
    }

    StaticSocketDataProvider reads(data_reads, data_writes);
    session_deps_.socket_factory->AddSocketDataProvider(&reads);

    TestCompletionCallback callback;
    auto session = CreateSession(&session_deps_);
    HttpNetworkTransaction trans(DEFAULT_PRIORITY, session.get());
    int rv = trans.Start(&request, callback.callback(), NetLogWithSource());
    EXPECT_THAT(rv, IsError(ERR_IO_PENDING));
    EXPECT_THAT(callback.WaitForResult(), IsOk());
  }
