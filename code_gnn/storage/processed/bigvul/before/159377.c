std::unique_ptr<net::test_server::HttpResponse> HandleSetCookieRequest(
    const net::test_server::HttpRequest& request) {
  if (!base::StartsWith(request.relative_url, "/set-cookie?",
                        base::CompareCase::SENSITIVE))
    return nullptr;

  std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
      new net::test_server::BasicHttpResponse);
  http_response->set_code(net::HTTP_OK);

  size_t query_string_pos = request.relative_url.find('?');
  std::string cookie_value =
      request.relative_url.substr(query_string_pos + 1);

  for (const std::string& cookie : base::SplitString(
           cookie_value, "&", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL))
    http_response->AddCustomHeader("Set-Cookie", cookie);

  return std::move(http_response);
}
