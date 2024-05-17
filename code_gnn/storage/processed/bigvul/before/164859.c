ServerRedirectRequestHandler(const net::test_server::HttpRequest& request) {
  if (!base::StartsWith(request.relative_url, "/redirect",
                        base::CompareCase::SENSITIVE)) {
    return std::unique_ptr<net::test_server::HttpResponse>();
  }

  std::unique_ptr<net::test_server::BasicHttpResponse> response(
      new net::test_server::BasicHttpResponse());
  size_t query_position = request.relative_url.find('?');

  if (query_position == std::string::npos) {
    response->set_code(net::HTTP_PERMANENT_REDIRECT);
    response->AddCustomHeader("Location",
                              "https://request-had-no-query-string");
    response->set_content_type("text/plain");
    response->set_content("Error");
    return std::move(response);
  }

  response->set_code(net::HTTP_PERMANENT_REDIRECT);
  response->AddCustomHeader("Location",
                            request.relative_url.substr(query_position + 1));
  response->set_content_type("text/plain");
  response->set_content("It's gone!");
  return std::move(response);
}
