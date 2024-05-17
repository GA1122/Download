void WebMediaPlayerImpl::PauseVideoIfNeeded() {
  DCHECK(IsHidden());

  if (!pipeline_controller_.IsPipelineRunning() || is_pipeline_resuming_ ||
      seeking_ || paused_)
    return;

  OnPause();
  paused_when_hidden_ = true;
}
