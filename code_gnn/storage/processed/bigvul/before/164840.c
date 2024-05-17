EchoReferrerRequestHandler(const net::test_server::HttpRequest& request) {
  const std::string kReferrerHeader = "Referer";   

  if (!base::StartsWith(request.relative_url, "/echoreferrer",
                        base::CompareCase::SENSITIVE)) {
    return std::unique_ptr<net::test_server::HttpResponse>();
  }

  std::unique_ptr<net::test_server::BasicHttpResponse> response(
      new net::test_server::BasicHttpResponse());
  response->set_code(net::HTTP_OK);
  response->set_content_type("text/plain");
  response->AddCustomHeader("Content-Disposition", "attachment");
  auto referrer_header = request.headers.find(kReferrerHeader);
  if (referrer_header != request.headers.end())
    response->set_content(referrer_header->second);
  return std::move(response);
}
