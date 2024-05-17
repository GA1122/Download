void MetricsWebContentsObserver::RenderProcessGone(
    base::TerminationStatus status) {
  if (status == base::TERMINATION_STATUS_NORMAL_TERMINATION ||
      status == base::TERMINATION_STATUS_STILL_RUNNING) {
    return;
  }

  if (committed_load_) {
    committed_load_->NotifyPageEnd(END_RENDER_PROCESS_GONE,
                                   UserInitiatedInfo::NotUserInitiated(),
                                   base::TimeTicks::Now(), true);
  }

  committed_load_.reset();
  aborted_provisional_loads_.clear();
}
