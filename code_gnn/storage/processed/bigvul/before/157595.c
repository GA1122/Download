void CheckContentEncodingMatching(SpdySessionDependencies* session_deps,
                                  const std::string& accept_encoding,
                                  const std::string& content_encoding,
                                  const std::string& location,
                                  bool should_match) {
  HttpRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.foo.com/");
  request.extra_headers.SetHeader(HttpRequestHeaders::kAcceptEncoding,
                                  accept_encoding);
  request.traffic_annotation =
      net::MutableNetworkTrafficAnnotationTag(TRAFFIC_ANNOTATION_FOR_TESTS);

  std::unique_ptr<HttpNetworkSession> session(CreateSession(session_deps));
  HttpNetworkTransaction trans(DEFAULT_PRIORITY, session.get());
  MockWrite data_writes[] = {
      MockWrite("GET / HTTP/1.1\r\n"
                "Host: www.foo.com\r\n"
                "Connection: keep-alive\r\n"
                "Accept-Encoding: "),
      MockWrite(accept_encoding.data()), MockWrite("\r\n\r\n"),
  };

  std::string response_code = "200 OK";
  std::string extra;
  if (!location.empty()) {
    response_code = "301 Redirect\r\nLocation: ";
    response_code.append(location);
  }

  MockRead data_reads[] = {
      MockRead("HTTP/1.0 "),
      MockRead(response_code.data()),
      MockRead("\r\nContent-Encoding: "),
      MockRead(content_encoding.data()),
      MockRead("\r\n\r\n"),
      MockRead(SYNCHRONOUS, OK),
  };
  StaticSocketDataProvider data(data_reads, data_writes);
  session_deps->socket_factory->AddSocketDataProvider(&data);

  TestCompletionCallback callback;

  int rv = trans.Start(&request, callback.callback(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  rv = callback.WaitForResult();
  if (should_match) {
    EXPECT_THAT(rv, IsOk());
  } else {
    EXPECT_THAT(rv, IsError(ERR_CONTENT_DECODING_FAILED));
  }
}
