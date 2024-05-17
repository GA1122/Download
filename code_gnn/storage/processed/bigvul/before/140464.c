int GroupNameTransactionHelper(
    const std::string& url,
    const scoped_refptr<HttpNetworkSession>& session) {
  HttpRequestInfo request;
  request.method = "GET";
  request.url = GURL(url);
  request.load_flags = 0;

  scoped_ptr<HttpTransaction> trans(
      new HttpNetworkTransaction(DEFAULT_PRIORITY, session.get()));

  TestCompletionCallback callback;

  return trans->Start(&request, callback.callback(), BoundNetLog());
}
