void RunTransactionTestWithResponse(net::HttpCache* cache,
                                    const MockTransaction& trans_info,
                                    std::string* response_headers) {
  net::HttpResponseInfo response;
  RunTransactionTestWithResponseInfo(cache, trans_info, &response);
  response.headers->GetNormalizedHeaders(response_headers);
}
