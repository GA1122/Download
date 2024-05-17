  net::ProxyServer origin() const {
    return config()->test_params()->proxies_for_http().front().proxy_server();
  }
