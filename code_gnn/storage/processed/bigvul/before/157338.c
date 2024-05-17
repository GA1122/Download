void WebMediaPlayerImpl::SwitchToLocalRenderer(
    MediaObserverClient::ReasonToSwitchToLocal reason) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  if (!disable_pipeline_auto_suspend_)
    return;   
  disable_pipeline_auto_suspend_ = false;

  CreateVideoDecodeStatsReporter();

  ScheduleRestart();
  if (client_)
    client_->MediaRemotingStopped(GetSwitchToLocalMessage(reason));
}
