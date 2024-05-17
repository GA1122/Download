std::unique_ptr<net::test_server::HttpResponse> HandleSetHeaderRequest(
    const net::test_server::HttpRequest& request) {
  if (!base::StartsWith(request.relative_url, "/set-header?",
                        base::CompareCase::SENSITIVE))
    return nullptr;

  size_t query_string_pos = request.relative_url.find('?');
  std::string escaped_header =
      request.relative_url.substr(query_string_pos + 1);

  std::string header = net::UnescapeURLComponent(
      escaped_header,
      net::UnescapeRule::NORMAL | net::UnescapeRule::SPACES |
          net::UnescapeRule::PATH_SEPARATORS |
          net::UnescapeRule::URL_SPECIAL_CHARS_EXCEPT_PATH_SEPARATORS);

  size_t colon_pos = header.find(':');
  if (colon_pos == std::string::npos)
    return std::unique_ptr<net::test_server::HttpResponse>();

  std::string header_name = header.substr(0, colon_pos);
  std::string header_value = header.substr(colon_pos + 2);

  std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
      new net::test_server::BasicHttpResponse);
  http_response->set_code(net::HTTP_OK);
  http_response->AddCustomHeader(header_name, header_value);
  return std::move(http_response);
}
