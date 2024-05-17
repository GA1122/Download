  explicit TestURLRequestContextWithProxy(const std::string& proxy)
      : TestURLRequestContext(true) {
    context_storage_.set_proxy_service(ProxyService::CreateFixed(proxy));
    Init();
  }
