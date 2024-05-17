  RenderProcessHost* Wait(base::TimeDelta timeout = base::TimeDelta::Max()) {
    if (!captured_render_process_host_) {
      base::OneShotTimer timer;
      timer.Start(FROM_HERE, timeout, run_loop_.QuitClosure());
      run_loop_.Run();
      timer.Stop();
    }
    return captured_render_process_host_;
  }
