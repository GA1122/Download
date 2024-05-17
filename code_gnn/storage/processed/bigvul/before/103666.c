bool BrowserRenderProcessHost::FastShutdownIfPossible() {
  if (run_renderer_in_process())
    return false;   

  if (!content::GetContentClient()->browser()->IsFastShutdownPossible())
    return false;

  if (!child_process_launcher_.get() ||
      child_process_launcher_->IsStarting() ||
      !GetHandle())
    return false;   

  if (!sudden_termination_allowed())
    return false;

  child_process_launcher_.reset();
  fast_shutdown_started_ = true;
  return true;
}
