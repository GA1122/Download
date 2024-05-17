  void Run() {
    net::ProxyService* proxy_service =
        url_request_context_getter_->GetURLRequestContext()->proxy_service();
    proxy_service->ResetConfigService(
        new net::ProxyConfigServiceFixed(proxy_config_));
    done_->Signal();
  }
