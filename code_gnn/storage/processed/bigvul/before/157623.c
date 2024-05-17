  SimpleGetHelperResult SimpleGetHelperForData(
      base::span<StaticSocketDataProvider*> providers) {
    SimpleGetHelperResult out;

    HttpRequestInfo request;
    request.method = "GET";
    request.url = GURL("http://www.example.org/");
    request.traffic_annotation =
        net::MutableNetworkTrafficAnnotationTag(TRAFFIC_ANNOTATION_FOR_TESTS);

    BoundTestNetLog log;
    session_deps_.net_log = log.bound().net_log();
    std::unique_ptr<HttpNetworkSession> session(CreateSession(&session_deps_));
    HttpNetworkTransaction trans(DEFAULT_PRIORITY, session.get());

    for (auto* provider : providers) {
      session_deps_.socket_factory->AddSocketDataProvider(provider);
    }

    TestCompletionCallback callback;

    EXPECT_TRUE(log.bound().IsCapturing());
    int rv = trans.Start(&request, callback.callback(), log.bound());
    EXPECT_THAT(rv, IsError(ERR_IO_PENDING));

    out.rv = callback.WaitForResult();
    out.total_received_bytes = trans.GetTotalReceivedBytes();
    out.total_sent_bytes = trans.GetTotalSentBytes();

    EXPECT_TRUE(trans.GetLoadTimingInfo(&out.load_timing_info));
    TestLoadTimingNotReused(out.load_timing_info, CONNECT_TIMING_HAS_DNS_TIMES);

    if (out.rv != OK)
      return out;

    const HttpResponseInfo* response = trans.GetResponseInfo();
    if (!response || !response->headers) {
      out.rv = ERR_UNEXPECTED;
      return out;
    }
    out.status_line = response->headers->GetStatusLine();

    EXPECT_EQ("127.0.0.1", response->socket_address.host());
    EXPECT_EQ(80, response->socket_address.port());

    bool got_endpoint =
        trans.GetRemoteEndpoint(&out.remote_endpoint_after_start);
    EXPECT_EQ(got_endpoint,
              out.remote_endpoint_after_start.address().size() > 0);

    rv = ReadTransaction(&trans, &out.response_data);
    EXPECT_THAT(rv, IsOk());

    TestNetLogEntry::List entries;
    log.GetEntries(&entries);
    size_t pos = ExpectLogContainsSomewhere(
        entries, 0, NetLogEventType::HTTP_TRANSACTION_SEND_REQUEST_HEADERS,
        NetLogEventPhase::NONE);
    ExpectLogContainsSomewhere(
        entries, pos, NetLogEventType::HTTP_TRANSACTION_READ_RESPONSE_HEADERS,
        NetLogEventPhase::NONE);

    std::string line;
    EXPECT_TRUE(entries[pos].GetStringValue("line", &line));
    EXPECT_EQ("GET / HTTP/1.1\r\n", line);

    HttpRequestHeaders request_headers;
    EXPECT_TRUE(trans.GetFullRequestHeaders(&request_headers));
    std::string value;
    EXPECT_TRUE(request_headers.GetHeader("Host", &value));
    EXPECT_EQ("www.example.org", value);
    EXPECT_TRUE(request_headers.GetHeader("Connection", &value));
    EXPECT_EQ("keep-alive", value);

    std::string response_headers;
    EXPECT_TRUE(GetHeaders(entries[pos].params.get(), &response_headers));
    EXPECT_EQ("['Host: www.example.org','Connection: keep-alive']",
              response_headers);

    out.total_received_bytes = trans.GetTotalReceivedBytes();
    EXPECT_EQ(out.total_sent_bytes, trans.GetTotalSentBytes());

    trans.GetConnectionAttempts(&out.connection_attempts);
    return out;
  }
