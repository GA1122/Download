WebMediaPlayerImpl::UpdatePlayState_ComputePlayState(bool is_remote,
                                                     bool can_auto_suspend,
                                                     bool is_suspended,
                                                     bool is_backgrounded) {
  PlayState result;

  bool must_suspend = delegate_->IsFrameClosed();
  bool is_stale = delegate_->IsStale(delegate_id_);

  if (stale_state_override_for_testing_.has_value() &&
      ready_state_ >= stale_state_override_for_testing_.value()) {
    is_stale = true;
  }

  bool has_error = IsNetworkStateError(network_state_);

  bool have_future_data =
      highest_ready_state_ >= WebMediaPlayer::kReadyStateHaveFutureData;

  bool background_suspended =
      can_auto_suspend && is_backgrounded && paused_ && have_future_data;

  bool idle_suspended =
      can_auto_suspend && is_stale && paused_ && !seeking_ && !overlay_enabled_;

  bool can_stay_suspended =
      (is_stale || have_future_data) && is_suspended && paused_ && !seeking_;

  result.is_suspended = is_remote || must_suspend || idle_suspended ||
                        background_suspended || can_stay_suspended;

  DVLOG(3) << __func__ << ": is_remote=" << is_remote
           << ", must_suspend=" << must_suspend
           << ", idle_suspended=" << idle_suspended
           << ", background_suspended=" << background_suspended
           << ", can_stay_suspended=" << can_stay_suspended
           << ", is_stale=" << is_stale
           << ", have_future_data=" << have_future_data
           << ", paused_=" << paused_ << ", seeking_=" << seeking_;


  bool backgrounded_video_has_no_remote_controls =
      IsBackgroundedSuspendEnabled() && !IsResumeBackgroundVideosEnabled() &&
      is_backgrounded && HasVideo();
  bool can_play = !has_error && !is_remote && have_future_data;
  bool has_remote_controls =
      HasAudio() && !backgrounded_video_has_no_remote_controls;
  bool alive = can_play && !must_suspend &&
               (!background_suspended || has_remote_controls);
  if (!alive) {
    result.delegate_state = DelegateState::GONE;
    result.is_idle = delegate_->IsIdle(delegate_id_);
  } else if (paused_) {
    result.delegate_state =
        ended_ ? DelegateState::GONE : DelegateState::PAUSED;
    result.is_idle = !seeking_;
  } else {
    result.delegate_state = DelegateState::PLAYING;
    result.is_idle = false;
  }

  result.is_memory_reporting_enabled =
      !has_error && can_play && !result.is_suspended && (!paused_ || seeking_);

  return result;
}
