void DesktopWindowTreeHostX11::UpdateWMUserTime(
    const ui::PlatformEvent& event) {
  if (!IsActive())
    return;

  ui::EventType type = ui::EventTypeFromNative(event);
  if (type == ui::ET_MOUSE_PRESSED ||
      type == ui::ET_KEY_PRESSED ||
      type == ui::ET_TOUCH_PRESSED) {
    unsigned long wm_user_time_ms = static_cast<unsigned long>(
        (ui::EventTimeFromNative(event) - base::TimeTicks()).InMilliseconds());
    XChangeProperty(xdisplay_, xwindow_, gfx::GetAtom("_NET_WM_USER_TIME"),
                    XA_CARDINAL, 32, PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&wm_user_time_ms),
                    1);
  }
}
