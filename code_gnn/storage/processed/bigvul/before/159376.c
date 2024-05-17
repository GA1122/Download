std::unique_ptr<net::test_server::HttpResponse> HandleServerRedirectRequest(
    const net::test_server::HttpRequest& request) {
  if (!base::StartsWith(request.relative_url, "/server-redirect?",
                        base::CompareCase::SENSITIVE))
    return nullptr;

  size_t query_string_pos = request.relative_url.find('?');
  std::string redirect_target =
      request.relative_url.substr(query_string_pos + 1);

  std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
      new net::test_server::BasicHttpResponse);
  http_response->set_code(net::HTTP_MOVED_PERMANENTLY);
  http_response->AddCustomHeader("Location", redirect_target);
  return std::move(http_response);
}
