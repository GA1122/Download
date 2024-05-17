void HttpNetworkTransactionTest::CheckErrorIsPassedBack(
    int error, IoMode mode) {
  net::HttpRequestInfo request_info;
  request_info.url = GURL("https://www.example.com/");
  request_info.method = "GET";
  request_info.load_flags = net::LOAD_NORMAL;

  SSLSocketDataProvider ssl_data(mode, OK);
  net::MockWrite data_writes[] = {
    net::MockWrite(mode, error),
  };
  net::StaticSocketDataProvider data(NULL, 0,
                                     data_writes, arraysize(data_writes));
  session_deps_.socket_factory->AddSocketDataProvider(&data);
  session_deps_.socket_factory->AddSSLSocketDataProvider(&ssl_data);

  scoped_refptr<HttpNetworkSession> session(CreateSession(&session_deps_));
  scoped_ptr<HttpTransaction> trans(
      new HttpNetworkTransaction(DEFAULT_PRIORITY, session.get()));

  TestCompletionCallback callback;
  int rv = trans->Start(&request_info, callback.callback(), net::BoundNetLog());
  if (rv == net::ERR_IO_PENDING)
    rv = callback.WaitForResult();
  ASSERT_EQ(error, rv);
}
