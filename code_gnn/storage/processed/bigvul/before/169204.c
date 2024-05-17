CrossSiteRedirectResponseHandler(const net::EmbeddedTestServer* test_server,
                                 const net::test_server::HttpRequest& request) {
  net::HttpStatusCode http_status_code;

  size_t length_of_chosen_prefix;
  std::string prefix_302("/cross-site/");
  std::string prefix_307("/cross-site-307/");
  if (base::StartsWith(request.relative_url, prefix_302,
                       base::CompareCase::SENSITIVE)) {
    http_status_code = net::HTTP_MOVED_PERMANENTLY;
    length_of_chosen_prefix = prefix_302.length();
  } else if (base::StartsWith(request.relative_url, prefix_307,
                              base::CompareCase::SENSITIVE)) {
    http_status_code = net::HTTP_TEMPORARY_REDIRECT;
    length_of_chosen_prefix = prefix_307.length();
  } else {
    return std::unique_ptr<net::test_server::HttpResponse>();
  }
  std::string params = request.relative_url.substr(length_of_chosen_prefix);

  size_t slash = params.find('/');
  if (slash == std::string::npos)
    return std::unique_ptr<net::test_server::HttpResponse>();

  GURL::Replacements replace_host;
  replace_host.SetHostStr(base::StringPiece(params).substr(0, slash));
  GURL redirect_server =
      test_server->base_url().ReplaceComponents(replace_host);

  std::string path = params.substr(slash + 1);
  GURL redirect_target(redirect_server.Resolve(path));
  DCHECK(redirect_target.is_valid());

  std::unique_ptr<net::test_server::BasicHttpResponse> http_response(
      new net::test_server::BasicHttpResponse);
  http_response->set_code(http_status_code);
  http_response->AddCustomHeader("Location", redirect_target.spec());
  return std::move(http_response);
}
