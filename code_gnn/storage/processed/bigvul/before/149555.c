  HandleCacheRedirectRequest(const net::test_server::HttpRequest& request) {
    if (!base::StartsWith(request.relative_url, "/cached-redirect?",
                          base::CompareCase::INSENSITIVE_ASCII)) {
      return std::unique_ptr<net::test_server::HttpResponse>();
    }

    GURL request_url = request.GetURL();
    std::string dest =
        net::UnescapeBinaryURLComponent(request_url.query_piece());

    auto http_response =
        std::make_unique<net::test_server::BasicHttpResponse>();
    http_response->set_code(net::HTTP_MOVED_PERMANENTLY);
    http_response->AddCustomHeader("Location", dest);
    http_response->set_content_type("text/html");
    http_response->set_content(base::StringPrintf(
        "<html><head></head><body>Redirecting to %s</body></html>",
        dest.c_str()));
    http_response->AddCustomHeader("Cache-Control", "max-age=6000");
    return http_response;
  }
