void WebMediaPlayerImpl::FlingingStopped() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DCHECK(disable_pipeline_auto_suspend_);
  disable_pipeline_auto_suspend_ = false;

  CreateVideoDecodeStatsReporter();

  ScheduleRestart();
}
