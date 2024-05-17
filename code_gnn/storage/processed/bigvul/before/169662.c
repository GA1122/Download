  RenderProcessReadyObserver(RenderProcessHost* render_process_host)
      : render_process_host_(render_process_host),
        quit_closure_(run_loop_.QuitClosure()) {
    render_process_host_->AddObserver(this);
  }
