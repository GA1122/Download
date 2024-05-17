void BrowserView::UpdateTitleBar() {
  frame_->UpdateWindowTitle();
  if (!loading_animation_timer_.IsRunning())
    frame_->UpdateWindowIcon();
}
