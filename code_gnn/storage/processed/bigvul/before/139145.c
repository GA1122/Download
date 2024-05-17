bool RenderProcessHostImpl::FastShutdownIfPossible() {
  if (run_renderer_in_process())
    return false;   

  if (!child_process_launcher_.get() || child_process_launcher_->IsStarting() ||
      !GetHandle())
    return false;   

  if (!SuddenTerminationAllowed())
    return false;

  if (GetWorkerRefCount() != 0) {
    if (survive_for_worker_start_time_.is_null())
      survive_for_worker_start_time_ = base::TimeTicks::Now();
    return false;
  }

  fast_shutdown_started_ = true;

  ProcessDied(false  , nullptr);
  return true;
}
