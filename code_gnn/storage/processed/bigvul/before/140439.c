void HttpNetworkTransactionTest::BypassHostCacheOnRefreshHelper(
    int load_flags) {
  HttpRequestInfo request;
  request.method = "GET";
  request.load_flags = load_flags;
  request.url = GURL("http://www.google.com/");

  session_deps_.host_resolver.reset(new MockCachingHostResolver);

  scoped_refptr<HttpNetworkSession> session(CreateSession(&session_deps_));
  scoped_ptr<HttpTransaction> trans(
      new HttpNetworkTransaction(DEFAULT_PRIORITY, session.get()));

  AddressList addrlist;
  TestCompletionCallback callback;
  int rv = session_deps_.host_resolver->Resolve(
      HostResolver::RequestInfo(HostPortPair("www.google.com", 80)),
      DEFAULT_PRIORITY,
      &addrlist,
      callback.callback(),
      NULL,
      BoundNetLog());
  EXPECT_EQ(ERR_IO_PENDING, rv);
  rv = callback.WaitForResult();
  EXPECT_EQ(OK, rv);

  rv = session_deps_.host_resolver->Resolve(
      HostResolver::RequestInfo(HostPortPair("www.google.com", 80)),
      DEFAULT_PRIORITY,
      &addrlist,
      callback.callback(),
      NULL,
      BoundNetLog());
  ASSERT_EQ(OK, rv);

  session_deps_.host_resolver->rules()->AddSimulatedFailure("www.google.com");

  MockRead data_reads[] = { MockRead(SYNCHRONOUS, ERR_UNEXPECTED) };
  StaticSocketDataProvider data(data_reads, arraysize(data_reads), NULL, 0);
  session_deps_.socket_factory->AddSocketDataProvider(&data);

  rv = trans->Start(&request, callback.callback(), BoundNetLog());
  ASSERT_EQ(ERR_IO_PENDING, rv);
  rv = callback.WaitForResult();

  EXPECT_EQ(ERR_NAME_NOT_RESOLVED, rv);
}
