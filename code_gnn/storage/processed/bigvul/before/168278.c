FullscreenControlHost* BrowserView::GetFullscreenControlHost() {
  if (!fullscreen_control_host_) {
    fullscreen_control_host_ =
        std::make_unique<FullscreenControlHost>(this, this);
  }

  return fullscreen_control_host_.get();
}
