  SetProxyConfigTask(base::WaitableEvent* done,
                     net::URLRequestContextGetter* url_request_context_getter,
                     const net::ProxyConfig& proxy_config)
      : done_(done),
        url_request_context_getter_(url_request_context_getter),
        proxy_config_(proxy_config) {
  }
