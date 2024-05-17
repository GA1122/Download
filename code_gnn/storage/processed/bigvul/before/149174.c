void LockScreenMediaControlsView::MediaSessionChanged(
    const base::Optional<base::UnguessableToken>& request_id) {
  if (!media_session_id_.has_value()) {
    media_session_id_ = request_id;
    return;
  }

  if (hide_controls_timer_->IsRunning() && request_id == media_session_id_) {
    hide_controls_timer_->Stop();
    enabled_actions_.clear();
  }

  if (request_id != media_session_id_) {
    hide_controls_timer_->Start(FROM_HERE, kNextMediaDelay,
                                hide_media_controls_);
  }
}
