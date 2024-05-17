void LockScreenMediaControlsView::Dismiss() {
  media_controller_remote_->Stop();
  hide_media_controls_.Run();
}
