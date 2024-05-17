std::unique_ptr<net::test_server::HttpResponse> IncrementRequestCount(
    const std::string& relative_url,
    int* request_count,
    const net::test_server::HttpRequest& request) {
  if (request.relative_url == relative_url)
    (*request_count)++;
  return std::make_unique<net::test_server::BasicHttpResponse>();
}
