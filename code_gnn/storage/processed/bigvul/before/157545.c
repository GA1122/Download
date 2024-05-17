  DataReductionProxyInterceptorEndToEndTest()
      : scoped_task_environment_(
            base::test::ScopedTaskEnvironment::MainThreadType::IO),
        context_(true),
        context_storage_(&context_) {}
