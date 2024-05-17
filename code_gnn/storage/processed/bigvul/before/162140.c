  RenderProcessHostIsReadyObserver(RenderProcessHost* render_process_host,
                                   base::OnceClosure task)
      : render_process_host_(render_process_host),
        task_(std::move(task)),
        weak_factory_(this) {
    render_process_host_->AddObserver(this);
    if (render_process_host_->IsReady())
      PostTask();
  }
