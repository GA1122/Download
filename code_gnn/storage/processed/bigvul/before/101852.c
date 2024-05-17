void Browser::TogglePresentationMode() {
  window_->SetPresentationMode(!window_->InPresentationMode());
  WindowFullscreenStateChanged();
}
