void HttpNetworkTransactionTest::PreconnectErrorResendRequestTest(
    const MockWrite* write_failure,
    const MockRead* read_failure,
    bool use_spdy) {
  HttpRequestInfo request;
  request.method = "GET";
  request.url = GURL("https://www.foo.com/");
  request.load_flags = 0;

  CapturingNetLog net_log;
  session_deps_.net_log = &net_log;
  scoped_refptr<HttpNetworkSession> session(CreateSession(&session_deps_));

  SSLSocketDataProvider ssl1(ASYNC, OK);
  SSLSocketDataProvider ssl2(ASYNC, OK);
  if (use_spdy) {
    ssl1.SetNextProto(GetParam());
    ssl2.SetNextProto(GetParam());
  }
  session_deps_.socket_factory->AddSSLSocketDataProvider(&ssl1);
  session_deps_.socket_factory->AddSSLSocketDataProvider(&ssl2);

  scoped_ptr<SpdyFrame> spdy_request(spdy_util_.ConstructSpdyGet(
      request.url.spec().c_str(), false, 1, DEFAULT_PRIORITY));
  scoped_ptr<SpdyFrame> spdy_response(
      spdy_util_.ConstructSpdyGetSynReply(NULL, 0, 1));
  scoped_ptr<SpdyFrame> spdy_data(
      spdy_util_.ConstructSpdyBodyFrame(1, "hello", 5, true));

  const char kHttpRequest[] = "GET / HTTP/1.1\r\n"
      "Host: www.foo.com\r\n"
      "Connection: keep-alive\r\n\r\n";
  const char kHttpResponse[] = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\n";
  const char kHttpData[] = "hello";

  std::vector<MockRead> data1_reads;
  std::vector<MockWrite> data1_writes;
  if (write_failure) {
    ASSERT_FALSE(read_failure);
    data1_writes.push_back(*write_failure);
    data1_reads.push_back(MockRead(ASYNC, OK));
  } else {
    ASSERT_TRUE(read_failure);
    if (use_spdy) {
      data1_writes.push_back(CreateMockWrite(*spdy_request));
    } else {
      data1_writes.push_back(MockWrite(kHttpRequest));
    }
    data1_reads.push_back(*read_failure);
  }

  StaticSocketDataProvider data1(&data1_reads[0], data1_reads.size(),
                                 &data1_writes[0], data1_writes.size());
  session_deps_.socket_factory->AddSocketDataProvider(&data1);

  std::vector<MockRead> data2_reads;
  std::vector<MockWrite> data2_writes;

  if (use_spdy) {
    data2_writes.push_back(CreateMockWrite(*spdy_request, 0, ASYNC));

    data2_reads.push_back(CreateMockRead(*spdy_response, 1, ASYNC));
    data2_reads.push_back(CreateMockRead(*spdy_data, 2, ASYNC));
    data2_reads.push_back(MockRead(ASYNC, OK, 3));
  } else {
    data2_writes.push_back(
        MockWrite(ASYNC, kHttpRequest, strlen(kHttpRequest), 0));

    data2_reads.push_back(
        MockRead(ASYNC, kHttpResponse, strlen(kHttpResponse), 1));
    data2_reads.push_back(MockRead(ASYNC, kHttpData, strlen(kHttpData), 2));
    data2_reads.push_back(MockRead(ASYNC, OK, 3));
  }
  OrderedSocketData data2(&data2_reads[0], data2_reads.size(),
                          &data2_writes[0], data2_writes.size());
  session_deps_.socket_factory->AddSocketDataProvider(&data2);

  net::SSLConfig ssl_config;
  session->ssl_config_service()->GetSSLConfig(&ssl_config);
  session->GetNextProtos(&ssl_config.next_protos);
  session->http_stream_factory()->PreconnectStreams(
      1, request, DEFAULT_PRIORITY, ssl_config, ssl_config);
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(1, GetIdleSocketCountInSSLSocketPool(session.get()));

  TestCompletionCallback callback;

  scoped_ptr<HttpTransaction> trans(
      new HttpNetworkTransaction(DEFAULT_PRIORITY, session.get()));

  int rv = trans->Start(&request, callback.callback(), BoundNetLog());
  EXPECT_EQ(ERR_IO_PENDING, rv);

  rv = callback.WaitForResult();
  EXPECT_EQ(OK, rv);

  LoadTimingInfo load_timing_info;
  EXPECT_TRUE(trans->GetLoadTimingInfo(&load_timing_info));
  TestLoadTimingNotReused(
      load_timing_info,
      CONNECT_TIMING_HAS_DNS_TIMES|CONNECT_TIMING_HAS_SSL_TIMES);

  const HttpResponseInfo* response = trans->GetResponseInfo();
  ASSERT_TRUE(response != NULL);

  EXPECT_TRUE(response->headers.get() != NULL);
  EXPECT_EQ("HTTP/1.1 200 OK", response->headers->GetStatusLine());

  std::string response_data;
  rv = ReadTransaction(trans.get(), &response_data);
  EXPECT_EQ(OK, rv);
  EXPECT_EQ(kHttpData, response_data);
}
