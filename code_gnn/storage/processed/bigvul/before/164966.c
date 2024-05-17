  explicit ScheduledResourceRequestAdapter(
      std::unique_ptr<network::ResourceScheduler::ScheduledResourceRequest>
          request)
      : request_(std::move(request)) {
    request_->set_resume_callback(base::BindOnce(
        &ScheduledResourceRequestAdapter::Resume, base::Unretained(this)));
  }
