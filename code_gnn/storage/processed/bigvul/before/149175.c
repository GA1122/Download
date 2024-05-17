void LockScreenMediaControlsView::MediaSessionInfoChanged(
    media_session::mojom::MediaSessionInfoPtr session_info) {
  if (hide_controls_timer_->IsRunning())
    return;

  if (!media_controls_enabled_.Run() || !session_info) {
    hide_media_controls_.Run();
  } else if (!IsDrawn() &&
             session_info->playback_state ==
                 media_session::mojom::MediaPlaybackState::kPaused) {
    hide_media_controls_.Run();
  } else if (!IsDrawn()) {
    show_media_controls_.Run();
  }

  if (IsDrawn()) {
    SetIsPlaying(session_info &&
                 session_info->playback_state ==
                     media_session::mojom::MediaPlaybackState::kPlaying);
  }
}
