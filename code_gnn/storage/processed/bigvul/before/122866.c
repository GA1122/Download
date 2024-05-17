bool RenderProcessHostImpl::FastShutdownIfPossible() {
  if (run_renderer_in_process())
    return false;   

  if (!GetContentClient()->browser()->IsFastShutdownPossible())
    return false;

  if (!child_process_launcher_.get() ||
      child_process_launcher_->IsStarting() ||
      !GetHandle())
    return false;   

  if (!SuddenTerminationAllowed())
    return false;

  ProcessDied(false  );
  fast_shutdown_started_ = true;
  return true;
}
