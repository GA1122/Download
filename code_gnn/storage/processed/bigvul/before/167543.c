void WebMediaPlayerImpl::UpdatePlayState() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

#if defined(OS_ANDROID)   
  bool is_remote = IsRemote();
  bool can_auto_suspend = true;
#else
  bool is_remote = false;
  bool can_auto_suspend = !disable_pipeline_auto_suspend_;
  if (IsStreaming()) {
    bool at_beginning =
        ready_state_ == WebMediaPlayer::kReadyStateHaveNothing ||
        CurrentTime() == 0.0;
    if (!at_beginning || GetPipelineMediaDuration() == kInfiniteDuration)
      can_auto_suspend = false;
  }
#endif

  bool is_suspended = pipeline_controller_.IsSuspended();
  bool is_backgrounded = IsBackgroundedSuspendEnabled() && IsHidden();
  PlayState state = UpdatePlayState_ComputePlayState(
      is_remote, can_auto_suspend, is_suspended, is_backgrounded);
  SetDelegateState(state.delegate_state, state.is_idle);
  SetMemoryReportingState(state.is_memory_reporting_enabled);
  SetSuspendState(state.is_suspended || pending_suspend_resume_cycle_);
}
