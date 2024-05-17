  DataReductionProxyInterceptorWithServerTest()
      : scoped_task_environment_(
            base::test::ScopedTaskEnvironment::MainThreadType::IO),
        context_(true) {
    context_.set_network_delegate(&network_delegate_);
  }
