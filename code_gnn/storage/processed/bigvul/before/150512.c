std::unique_ptr<net::test_server::HttpResponse> CreateDRPResponseWithHeader(
    const net::test_server::HttpRequest& request) {
  auto response = std::make_unique<net::test_server::BasicHttpResponse>();
  const auto chrome_proxy_header = request.headers.find("chrome-proxy");
  if (chrome_proxy_header != request.headers.end())
    response->set_content(chrome_proxy_header->second);
  response->set_content_type("text/html");
  response->AddCustomHeader("chrome-proxy", "ofcl=10");
  response->AddCustomHeader("via", "1.1 Chrome-Compression-Proxy");
  return response;
}
