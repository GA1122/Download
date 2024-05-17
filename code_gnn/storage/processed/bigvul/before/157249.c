void WebMediaPlayerImpl::OnBeforePipelineResume() {
  if (skip_metrics_due_to_startup_suspend_) {
    if (!attempting_suspended_start_)
      load_start_time_ = base::TimeTicks::Now() - time_to_metadata_;
    skip_metrics_due_to_startup_suspend_ = false;
  }

  EnableVideoTrackIfNeeded();
  is_pipeline_resuming_ = true;
}
