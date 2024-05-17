void HttpNetworkTransactionTest::PreconnectErrorResendRequestTest(
    const MockWrite* write_failure,
    const MockRead* read_failure,
    bool use_spdy) {
  HttpRequestInfo request;
  request.method = "GET";
  request.url = GURL("https://www.foo.com/");
  request.traffic_annotation =
      net::MutableNetworkTrafficAnnotationTag(TRAFFIC_ANNOTATION_FOR_TESTS);

  TestNetLog net_log;
  session_deps_.net_log = &net_log;
  std::unique_ptr<HttpNetworkSession> session(CreateSession(&session_deps_));

  SSLSocketDataProvider ssl1(ASYNC, OK);
  SSLSocketDataProvider ssl2(ASYNC, OK);
  if (use_spdy) {
    ssl1.next_proto = kProtoHTTP2;
    ssl2.next_proto = kProtoHTTP2;
  }
  session_deps_.socket_factory->AddSSLSocketDataProvider(&ssl1);
  session_deps_.socket_factory->AddSSLSocketDataProvider(&ssl2);

  spdy::SpdySerializedFrame spdy_request(spdy_util_.ConstructSpdyGet(
      request.url.spec().c_str(), 1, DEFAULT_PRIORITY));
  spdy::SpdySerializedFrame spdy_response(
      spdy_util_.ConstructSpdyGetReply(NULL, 0, 1));
  spdy::SpdySerializedFrame spdy_data(
      spdy_util_.ConstructSpdyDataFrame(1, "hello", true));

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
      data1_writes.push_back(CreateMockWrite(spdy_request));
    } else {
      data1_writes.push_back(MockWrite(kHttpRequest));
    }
    data1_reads.push_back(*read_failure);
  }

  StaticSocketDataProvider data1(data1_reads, data1_writes);
  session_deps_.socket_factory->AddSocketDataProvider(&data1);

  std::vector<MockRead> data2_reads;
  std::vector<MockWrite> data2_writes;

  if (use_spdy) {
    data2_writes.push_back(CreateMockWrite(spdy_request, 0, ASYNC));

    data2_reads.push_back(CreateMockRead(spdy_response, 1, ASYNC));
    data2_reads.push_back(CreateMockRead(spdy_data, 2, ASYNC));
    data2_reads.push_back(MockRead(ASYNC, OK, 3));
  } else {
    data2_writes.push_back(
        MockWrite(ASYNC, kHttpRequest, strlen(kHttpRequest), 0));

    data2_reads.push_back(
        MockRead(ASYNC, kHttpResponse, strlen(kHttpResponse), 1));
    data2_reads.push_back(MockRead(ASYNC, kHttpData, strlen(kHttpData), 2));
    data2_reads.push_back(MockRead(ASYNC, OK, 3));
  }
  SequencedSocketData data2(data2_reads, data2_writes);
  session_deps_.socket_factory->AddSocketDataProvider(&data2);

  session->http_stream_factory()->PreconnectStreams(1, request);
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(1, GetIdleSocketCountInSSLSocketPool(session.get()));

  TestCompletionCallback callback;

  HttpNetworkTransaction trans(DEFAULT_PRIORITY, session.get());

  int rv = trans.Start(&request, callback.callback(), NetLogWithSource());
  EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

  rv = callback.WaitForResult();
  EXPECT_THAT(rv, IsOk());

  LoadTimingInfo load_timing_info;
  EXPECT_TRUE(trans.GetLoadTimingInfo(&load_timing_info));
  TestLoadTimingNotReused(
      load_timing_info,
      CONNECT_TIMING_HAS_DNS_TIMES|CONNECT_TIMING_HAS_SSL_TIMES);

  const HttpResponseInfo* response = trans.GetResponseInfo();
  ASSERT_TRUE(response);

  EXPECT_TRUE(response->headers);
  if (response->was_fetched_via_spdy) {
    EXPECT_EQ("HTTP/1.1 200", response->headers->GetStatusLine());
  } else {
    EXPECT_EQ("HTTP/1.1 200 OK", response->headers->GetStatusLine());
  }

  std::string response_data;
  rv = ReadTransaction(&trans, &response_data);
  EXPECT_THAT(rv, IsOk());
  EXPECT_EQ(kHttpData, response_data);
}
