  TestURLRequestContextWithDataReductionProxy(const net::ProxyServer& origin,
                                              net::NetworkDelegate* delegate)
      : net::TestURLRequestContext(true) {
    context_storage_.set_proxy_resolution_service(
        net::ProxyResolutionService::CreateFixed(origin.ToURI(),
                                                 TRAFFIC_ANNOTATION_FOR_TESTS));
    set_network_delegate(delegate);
  }
