void DesktopWindowTreeHostX11::ReleaseCapture() {
  if (g_current_capture == this) {
    g_current_capture = nullptr;
    ui::UngrabPointer();
    has_pointer_grab_ = false;

    OnHostLostWindowCapture();
  }
}
