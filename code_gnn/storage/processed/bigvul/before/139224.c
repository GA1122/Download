bool RenderProcessHostImpl::Shutdown(int exit_code, bool wait) {
  if (run_renderer_in_process())
    return false;   

  if (!child_process_launcher_.get())
    return false;

  return child_process_launcher_->Terminate(exit_code, wait);
}
