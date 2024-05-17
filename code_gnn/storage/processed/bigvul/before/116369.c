void RunTransactionTestWithRequest(net::HttpCache* cache,
                                   const MockTransaction& trans_info,
                                   const MockHttpRequest& request,
                                   net::HttpResponseInfo* response_info) {
  RunTransactionTestWithRequestAndLog(cache, trans_info, request,
                                      response_info, net::BoundNetLog());
}
