  TestURLRequestContextWithProxy(const std::string& proxy,
                                 NetworkDelegate* delegate)
      : TestURLRequestContext(true) {
    context_storage_.set_proxy_service(ProxyService::CreateFixed(proxy));
    set_network_delegate(delegate);
    Init();
  }
