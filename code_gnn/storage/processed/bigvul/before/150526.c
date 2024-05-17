std::unique_ptr<net::test_server::HttpResponse> RespondWithRequestPathHandler(
    const std::string& server_name,
    EventLog* event_log,
    const net::test_server::HttpRequest& request) {
  if (request.relative_url == "/favicon.ico")
    return nullptr;

  event_log->Add(server_name + " responded 200 for " + request.relative_url);
  auto response = std::make_unique<net::test_server::BasicHttpResponse>();
  response->set_content_type("text/plain");
  response->set_code(net::HTTP_OK);
  response->set_content(request.relative_url);
  return response;
}
