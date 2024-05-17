void WebMediaPlayerImpl::ScheduleIdlePauseTimer() {
  if ((paused_ && !paused_when_hidden_) ||
      !pipeline_controller_.IsSuspended() || !HasAudio()) {
    return;
  }

#if defined(OS_ANDROID)
  if (IsRemote())
    return;
#endif

  background_pause_timer_.Start(FROM_HERE, base::TimeDelta::FromSeconds(5),
                                this, &WebMediaPlayerImpl::OnPause);
}
