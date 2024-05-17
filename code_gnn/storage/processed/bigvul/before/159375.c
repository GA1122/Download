std::unique_ptr<net::test_server::HttpResponse> HandleEchoHeaderRequest(
    const net::test_server::HttpRequest& request) {
  if (!base::StartsWith(request.relative_url, "/echoheader?",
                        base::CompareCase::SENSITIVE))
    return nullptr;

  size_t query_string_pos = request.relative_url.find('?');
  std::string header_name =
      request.relative_url.substr(query_string_pos + 1);

  std::string header_value;
  auto it = request.headers.find(header_name);
  if (it != request.headers.end())
    header_value = it->second;

  std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
      new net::test_server::BasicHttpResponse);
  http_response->set_code(net::HTTP_OK);
  http_response->set_content(header_value);
  return std::move(http_response);
}
