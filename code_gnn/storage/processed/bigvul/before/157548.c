  void Init(std::unique_ptr<net::URLRequestJobFactory> factory) {
    job_factory_ = std::move(factory);
    default_context_->set_job_factory(job_factory_.get());
    default_context_->Init();
  }
