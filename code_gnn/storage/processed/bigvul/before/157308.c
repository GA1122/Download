void WebMediaPlayerImpl::ScheduleRestart() {
  if (pipeline_controller_.IsPipelineRunning() &&
      !pipeline_controller_.IsPipelineSuspended()) {
    pending_suspend_resume_cycle_ = true;
    UpdatePlayState();
  }
}
