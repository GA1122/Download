int GroupNameTransactionHelper(const std::string& url,
                               HttpNetworkSession* session) {
  HttpRequestInfo request;
  request.method = "GET";
  request.url = GURL(url);
  request.traffic_annotation =
      net::MutableNetworkTrafficAnnotationTag(TRAFFIC_ANNOTATION_FOR_TESTS);

  HttpNetworkTransaction trans(DEFAULT_PRIORITY, session);

  TestCompletionCallback callback;

  return trans.Start(&request, callback.callback(), NetLogWithSource());
}
