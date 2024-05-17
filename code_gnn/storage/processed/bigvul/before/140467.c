void HttpNetworkTransactionTest::KeepAliveConnectionResendRequestTest(
    const MockWrite* write_failure,
    const MockRead* read_failure) {
  HttpRequestInfo request;
  request.method = "GET";
  request.url = GURL("http://www.foo.com/");
  request.load_flags = 0;

  CapturingNetLog net_log;
  session_deps_.net_log = &net_log;
  scoped_refptr<HttpNetworkSession> session(CreateSession(&session_deps_));

  MockWrite data1_writes[] = {
    MockWrite("GET / HTTP/1.1\r\n"
              "Host: www.foo.com\r\n"
              "Connection: keep-alive\r\n\r\n"),
    MockWrite("GET / HTTP/1.1\r\n"
              "Host: www.foo.com\r\n"
              "Connection: keep-alive\r\n\r\n")
  };

  MockRead data1_reads[] = {
    MockRead("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\n"),
    MockRead("hello"),
    MockRead(ASYNC, OK),
  };

  if (write_failure) {
    ASSERT_FALSE(read_failure);
    data1_writes[1] = *write_failure;
  } else {
    ASSERT_TRUE(read_failure);
    data1_reads[2] = *read_failure;
  }

  StaticSocketDataProvider data1(data1_reads, arraysize(data1_reads),
                                 data1_writes, arraysize(data1_writes));
  session_deps_.socket_factory->AddSocketDataProvider(&data1);

  MockRead data2_reads[] = {
    MockRead("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\n"),
    MockRead("world"),
    MockRead(ASYNC, OK),
  };
  StaticSocketDataProvider data2(data2_reads, arraysize(data2_reads), NULL, 0);
  session_deps_.socket_factory->AddSocketDataProvider(&data2);

  const char* kExpectedResponseData[] = {
    "hello", "world"
  };

  uint32 first_socket_log_id = NetLog::Source::kInvalidId;
  for (int i = 0; i < 2; ++i) {
    TestCompletionCallback callback;

    scoped_ptr<HttpTransaction> trans(
        new HttpNetworkTransaction(DEFAULT_PRIORITY, session.get()));

    int rv = trans->Start(&request, callback.callback(), BoundNetLog());
    EXPECT_EQ(ERR_IO_PENDING, rv);

    rv = callback.WaitForResult();
    EXPECT_EQ(OK, rv);

    LoadTimingInfo load_timing_info;
    EXPECT_TRUE(trans->GetLoadTimingInfo(&load_timing_info));
    TestLoadTimingNotReused(load_timing_info, CONNECT_TIMING_HAS_DNS_TIMES);
    if (i == 0) {
      first_socket_log_id = load_timing_info.socket_log_id;
    } else {
      EXPECT_NE(first_socket_log_id, load_timing_info.socket_log_id);
    }

    const HttpResponseInfo* response = trans->GetResponseInfo();
    ASSERT_TRUE(response != NULL);

    EXPECT_TRUE(response->headers.get() != NULL);
    EXPECT_EQ("HTTP/1.1 200 OK", response->headers->GetStatusLine());

    std::string response_data;
    rv = ReadTransaction(trans.get(), &response_data);
    EXPECT_EQ(OK, rv);
    EXPECT_EQ(kExpectedResponseData[i], response_data);
  }
}
