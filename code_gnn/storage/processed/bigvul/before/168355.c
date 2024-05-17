bool BrowserView::ShouldHideUIForFullscreen() const {
  if (immersive_mode_controller_->IsEnabled())
    return false;

  return frame_->GetFrameView()->ShouldHideTopUIForFullscreen();
}
