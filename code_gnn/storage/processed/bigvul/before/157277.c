void WebMediaPlayerImpl::OnProgress() {
  DVLOG(4) << __func__;
  if (highest_ready_state_ < ReadyState::kReadyStateHaveFutureData) {
    preroll_attempt_pending_ = true;
    preroll_attempt_start_time_ = base::TimeTicks();

    delegate_->ClearStaleFlag(delegate_id_);
    UpdatePlayState();
  } else if (ready_state_ == ReadyState::kReadyStateHaveFutureData &&
             CanPlayThrough()) {
    SetReadyState(WebMediaPlayer::kReadyStateHaveEnoughData);
  }
}
