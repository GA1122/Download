std::unique_ptr<net::test_server::HttpResponse> CustomHandleRequestForCookies(
    const net::test_server::HttpRequest& request) {
  if (request.relative_url == "/index.html") {
    std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
        new net::test_server::BasicHttpResponse());
    http_response->set_code(net::HTTP_OK);
    http_response->set_content_type("text/html");
    http_response->set_content(
        "<html><head>"
        "<link rel=manifest crossorigin='use-credentials' href=/manifest.json>"
        "</head></html>");
    return std::move(http_response);
  }

  const auto& iter = request.headers.find("Cookie");
  if (iter == request.headers.end() || request.relative_url != "/manifest.json")
    return std::unique_ptr<net::test_server::HttpResponse>();

  std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
      new net::test_server::BasicHttpResponse());
  http_response->set_code(net::HTTP_OK);
  http_response->set_content_type("application/json");
  http_response->set_content(
      base::StringPrintf("{\"name\": \"%s\"}", iter->second.c_str()));

  return std::move(http_response);
}
