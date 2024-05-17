void RunTransactionTestWithLog(net::HttpCache* cache,
                               const MockTransaction& trans_info,
                               const net::BoundNetLog& log) {
  RunTransactionTestWithRequestAndLog(
      cache, trans_info, MockHttpRequest(trans_info), NULL, log);
}
