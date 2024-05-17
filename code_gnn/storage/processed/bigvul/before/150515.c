std::unique_ptr<net::test_server::HttpResponse> DrpBlockOnceHandler(
    const std::string& server_name,
    EventLog* event_log,
    const net::test_server::HttpRequest& request) {
  if (request.relative_url == "/favicon.ico")
    return nullptr;

  event_log->Add(server_name + " responded 502 for " + request.relative_url);
  auto response = std::make_unique<net::test_server::BasicHttpResponse>();
  response->set_content_type("text/plain");
  response->set_code(net::HTTP_BAD_GATEWAY);
  response->AddCustomHeader(chrome_proxy_header(), "block-once");
  return response;
}
