  SimpleGetHelperResult SimpleGetHelperForData(StaticSocketDataProvider* data[],
                                               size_t data_count) {
    SimpleGetHelperResult out;

    HttpRequestInfo request;
    request.method = "GET";
    request.url = GURL("http://www.google.com/");
    request.load_flags = 0;

    CapturingBoundNetLog log;
    session_deps_.net_log = log.bound().net_log();
    scoped_refptr<HttpNetworkSession> session(CreateSession(&session_deps_));
    scoped_ptr<HttpTransaction> trans(
        new HttpNetworkTransaction(DEFAULT_PRIORITY, session.get()));

    for (size_t i = 0; i < data_count; ++i) {
      session_deps_.socket_factory->AddSocketDataProvider(data[i]);
    }

    TestCompletionCallback callback;

    EXPECT_TRUE(log.bound().IsLogging());
    int rv = trans->Start(&request, callback.callback(), log.bound());
    EXPECT_EQ(ERR_IO_PENDING, rv);

    out.rv = callback.WaitForResult();

    EXPECT_TRUE(trans->GetLoadTimingInfo(&out.load_timing_info));
    TestLoadTimingNotReused(out.load_timing_info, CONNECT_TIMING_HAS_DNS_TIMES);

    if (out.rv != OK)
      return out;

    const HttpResponseInfo* response = trans->GetResponseInfo();
    if (response == NULL || response->headers.get() == NULL) {
      out.rv = ERR_UNEXPECTED;
      return out;
    }
    out.status_line = response->headers->GetStatusLine();

    EXPECT_EQ("127.0.0.1", response->socket_address.host());
    EXPECT_EQ(80, response->socket_address.port());

    rv = ReadTransaction(trans.get(), &out.response_data);
    EXPECT_EQ(OK, rv);

    net::CapturingNetLog::CapturedEntryList entries;
    log.GetEntries(&entries);
    size_t pos = ExpectLogContainsSomewhere(
        entries, 0, NetLog::TYPE_HTTP_TRANSACTION_SEND_REQUEST_HEADERS,
        NetLog::PHASE_NONE);
    ExpectLogContainsSomewhere(
        entries, pos,
        NetLog::TYPE_HTTP_TRANSACTION_READ_RESPONSE_HEADERS,
        NetLog::PHASE_NONE);

    std::string line;
    EXPECT_TRUE(entries[pos].GetStringValue("line", &line));
    EXPECT_EQ("GET / HTTP/1.1\r\n", line);

    HttpRequestHeaders request_headers;
    EXPECT_TRUE(trans->GetFullRequestHeaders(&request_headers));
    std::string value;
    EXPECT_TRUE(request_headers.GetHeader("Host", &value));
    EXPECT_EQ("www.google.com", value);
    EXPECT_TRUE(request_headers.GetHeader("Connection", &value));
    EXPECT_EQ("keep-alive", value);

    std::string response_headers;
    EXPECT_TRUE(GetHeaders(entries[pos].params.get(), &response_headers));
    EXPECT_EQ("['Host: www.google.com','Connection: keep-alive']",
              response_headers);

    out.totalReceivedBytes = trans->GetTotalReceivedBytes();
    return out;
  }
