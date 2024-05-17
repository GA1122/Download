ProxyResolverV8TracingImpl::ProxyResolverV8TracingImpl(
    scoped_ptr<base::Thread> thread,
    scoped_ptr<ProxyResolverV8> resolver,
    scoped_ptr<Job::Params> job_params)
    : thread_(std::move(thread)),
      v8_resolver_(std::move(resolver)),
      job_params_(std::move(job_params)),
      num_outstanding_callbacks_(0) {
  job_params_->num_outstanding_callbacks = &num_outstanding_callbacks_;
}
