  TestJobInterceptor* AddTestInterceptor() {
    TestJobInterceptor* interceptor = new TestJobInterceptor();
    default_context_.set_job_factory(&job_factory_);
    job_factory_.AddInterceptor(interceptor);
    return interceptor;
  }
