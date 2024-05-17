void DesktopWindowTreeHostX11::SetCapture() {
  if (HasCapture())
    return;

  DesktopWindowTreeHostX11* old_capturer = g_current_capture;

  g_current_capture = this;
  if (old_capturer)
    old_capturer->OnHostLostWindowCapture();

  has_pointer_grab_ |= !ui::GrabPointer(xwindow_, true, x11::None);
}
