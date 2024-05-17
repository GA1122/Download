void WebMediaPlayerImpl::OnEnded() {
  TRACE_EVENT2("media", "WebMediaPlayerImpl::OnEnded", "duration", Duration(),
               "id", media_log_->id());
  DVLOG(1) << __func__;
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (!pipeline_controller_.IsStable())
    return;

  ended_ = true;
  client_->TimeChanged();

  frame_time_report_cb_.Cancel();

  UpdatePlayState();
}
