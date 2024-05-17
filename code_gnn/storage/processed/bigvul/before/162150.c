void RenderProcessHostImpl::ShutdownRequest() {
  for (auto& observer : observers_) {
    observer.RenderProcessShutdownRequested(this);
  }

  if (pending_views_ || run_renderer_in_process() || GetActiveViewCount() > 0) {
    return;
  }

  for (auto& observer : observers_) {
    observer.RenderProcessWillExit(this);
  }
  child_control_interface_->ProcessShutdown();
}
