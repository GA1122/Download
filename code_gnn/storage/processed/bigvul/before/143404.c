ProxyResolverV8TracingFactoryImpl::~ProxyResolverV8TracingFactoryImpl() {
  for (auto job : jobs_) {
    job->FactoryDestroyed();
  }
}
