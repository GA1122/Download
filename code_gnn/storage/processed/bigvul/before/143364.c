  CreateJob(ProxyResolverV8TracingFactoryImpl* factory,
            scoped_ptr<ProxyResolverV8Tracing::Bindings> bindings,
            const scoped_refptr<ProxyResolverScriptData>& pac_script,
            scoped_ptr<ProxyResolverV8Tracing>* resolver_out,
            const CompletionCallback& callback)
      : factory_(factory),
        thread_(new base::Thread("Proxy Resolver")),
        resolver_out_(resolver_out),
        callback_(callback),
        num_outstanding_callbacks_(0) {
    base::Thread::Options options;
    options.timer_slack = base::TIMER_SLACK_MAXIMUM;
    CHECK(thread_->StartWithOptions(options));
    job_params_.reset(
        new Job::Params(thread_->task_runner(), &num_outstanding_callbacks_));
    create_resolver_job_ = new Job(job_params_.get(), std::move(bindings));
    create_resolver_job_->StartCreateV8Resolver(
        pac_script, &v8_resolver_,
        base::Bind(
            &ProxyResolverV8TracingFactoryImpl::CreateJob::OnV8ResolverCreated,
            base::Unretained(this)));
  }
