  void OnBeforeProxyHeadersSent(const ProxyInfo& proxy_info,
                                HttpRequestHeaders* request_headers) {
    observed_before_proxy_headers_sent_ = true;
    observed_proxy_server_uri_ = proxy_info.proxy_server().ToURI();
  }
