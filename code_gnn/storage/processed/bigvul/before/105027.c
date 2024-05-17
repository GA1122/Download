  FakeURLFetcher(const GURL& url, RequestType request_type, Delegate* d,
                 const std::string& response_data, bool success)
    : URLFetcher(url, request_type, d),
      url_(url),
      response_data_(response_data),
      success_(success),
      ALLOW_THIS_IN_INITIALIZER_LIST(method_factory_(this)) {
  }
