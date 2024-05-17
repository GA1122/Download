  SetProxyConfigTask(URLRequestContextGetter* request_context_getter,
                     const std::string& new_proxy_config)
      : request_context_getter_(request_context_getter),
        proxy_config_(new_proxy_config) {}
