  void OnBeforeHeadersSent(const ProxyInfo& proxy_info,
                           HttpRequestHeaders* request_headers) {
    observed_before_headers_sent_ = true;
    if (!proxy_info.is_http() && !proxy_info.is_https() &&
        !proxy_info.is_quic()) {
      return;
    }
    observed_before_headers_sent_with_proxy_ = true;
    observed_proxy_server_uri_ = proxy_info.proxy_server().ToURI();
  }
