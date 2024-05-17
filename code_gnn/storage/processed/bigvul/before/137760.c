void HeadlessWebContentsImpl::InitializeWindow(
    const gfx::Rect& initial_bounds) {
  static int window_id = 1;
  window_id_ = window_id++;
  window_state_ = "normal";

  browser()->PlatformInitializeWebContents(this);
  SetBounds(initial_bounds);

  if (begin_frame_control_enabled_) {
    ui::Compositor* compositor = browser()->PlatformGetCompositor(this);
    DCHECK(compositor);
    compositor->SetExternalBeginFrameClient(this);
  }
}
