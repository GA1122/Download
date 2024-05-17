Job::Job(const Job::Params* params,
         scoped_ptr<ProxyResolverV8Tracing::Bindings> bindings)
    : origin_runner_(base::ThreadTaskRunnerHandle::Get()),
      params_(params),
      bindings_(std::move(bindings)),
      event_(true, false),
      last_num_dns_(0),
      pending_dns_(NULL) {
  CheckIsOnOriginThread();
}
