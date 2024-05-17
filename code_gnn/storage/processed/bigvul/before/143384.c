  void OnV8ResolverCreated(int error) {
    DCHECK(factory_);
    if (error == OK) {
      job_params_->v8_resolver = v8_resolver_.get();
      resolver_out_->reset(new ProxyResolverV8TracingImpl(
          std::move(thread_), std::move(v8_resolver_), std::move(job_params_)));
    } else {
      StopWorkerThread();
    }

    factory_->RemoveJob(this);
    factory_ = nullptr;
    create_resolver_job_ = nullptr;
    callback_.Run(error);
  }
