void RunTransactionTestWithResponseInfo(net::HttpCache* cache,
                                        const MockTransaction& trans_info,
                                        net::HttpResponseInfo* response) {
  RunTransactionTestWithRequest(
      cache, trans_info, MockHttpRequest(trans_info), response);
}
