  std::unique_ptr<net::test_server::HttpResponse> AddChromeProxyHeader(
      const net::test_server::HttpRequest& request) {
    base::AutoLock auto_lock(lock_);
    auto response = std::make_unique<net::test_server::BasicHttpResponse>();
    if (!header_.empty())
      response->AddCustomHeader(chrome_proxy_header(), header_);
    if (!location_header_.empty())
      response->AddCustomHeader("Location", location_header_);
    if (response_hook_)
      response_hook_.Run(response.get());

    bool use_special_status_code = false;
    if (request.headers.find("Host") != request.headers.end()) {
      const GURL kOriginUrl(
          base::StrCat({"http://", request.headers.find("Host")->second +
                                       request.GetURL().path()}));

      if (!special_host_port_pair_.IsEmpty() &&
          net::HostPortPair::FromURL(kOriginUrl) == special_host_port_pair_) {
        use_special_status_code = true;
      }
    }

    if (use_special_status_code) {
      response->set_code(special_status_code_);
    } else {
      response->set_code(status_code_);
    }
    response->set_content(kPrimaryResponse);
    response->set_content_type("text/plain");
    return response;
  }
