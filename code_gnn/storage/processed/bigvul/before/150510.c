std::unique_ptr<net::test_server::HttpResponse> BasicResponse(
    const std::string& content,
    const net::test_server::HttpRequest& request) {
  auto response = std::make_unique<net::test_server::BasicHttpResponse>();
  response->set_content(content);
  response->set_content_type("text/plain");
  return response;
}
