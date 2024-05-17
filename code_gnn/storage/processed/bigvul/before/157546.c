  DataReductionProxyInterceptorTest()
      : scoped_task_environment_(
            base::test::ScopedTaskEnvironment::MainThreadType::IO) {
    test_context_ =
        DataReductionProxyTestContext::Builder()
            .Build();
    default_context_.reset(new TestURLRequestContextWithDataReductionProxy(
        test_context_->config()
            ->test_params()
            ->proxies_for_http()
            .front()
            .proxy_server(),
        &default_network_delegate_));
    default_context_->set_network_delegate(&default_network_delegate_);
    test_context_->config()->test_params()->UseNonSecureProxiesForHttp();
  }
