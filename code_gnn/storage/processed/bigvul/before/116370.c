void RunTransactionTestWithRequestAndLog(net::HttpCache* cache,
                                         const MockTransaction& trans_info,
                                         const MockHttpRequest& request,
                                         net::HttpResponseInfo* response_info,
                                         const net::BoundNetLog& net_log) {
  net::TestCompletionCallback callback;


  scoped_ptr<net::HttpTransaction> trans;
  int rv = cache->CreateTransaction(&trans);
  EXPECT_EQ(net::OK, rv);
  ASSERT_TRUE(trans.get());

  rv = trans->Start(&request, callback.callback(), net_log);
  if (rv == net::ERR_IO_PENDING)
    rv = callback.WaitForResult();
  ASSERT_EQ(net::OK, rv);

  const net::HttpResponseInfo* response = trans->GetResponseInfo();
  ASSERT_TRUE(response);

  if (response_info)
    *response_info = *response;

  ReadAndVerifyTransaction(trans.get(), trans_info);
}
