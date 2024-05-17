  void FactoryDestroyed() {
    factory_ = nullptr;
    create_resolver_job_->Cancel();
    create_resolver_job_ = nullptr;
    StopWorkerThread();
  }
