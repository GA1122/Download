void DesktopWindowTreeHostX11::MapWindow(ui::WindowShowState show_state) {
  if (show_state != ui::SHOW_STATE_DEFAULT &&
      show_state != ui::SHOW_STATE_NORMAL &&
      show_state != ui::SHOW_STATE_INACTIVE &&
      show_state != ui::SHOW_STATE_MAXIMIZED) {
    NOTIMPLEMENTED_LOG_ONCE();
  }

  XSizeHints size_hints;
  size_hints.flags = 0;
  long supplied_return;
  XGetWMNormalHints(xdisplay_, xwindow_, &size_hints, &supplied_return);
  size_hints.flags |= PPosition;
  size_hints.x = bounds_in_pixels_.x();
  size_hints.y = bounds_in_pixels_.y();
  XSetWMNormalHints(xdisplay_, xwindow_, &size_hints);

  ignore_keyboard_input_ = show_state == ui::SHOW_STATE_INACTIVE;
  unsigned long wm_user_time_ms =
      ignore_keyboard_input_
          ? 0
          : ui::X11EventSource::GetInstance()->GetTimestamp();
  if (show_state == ui::SHOW_STATE_INACTIVE || wm_user_time_ms != 0) {
    XChangeProperty(xdisplay_, xwindow_, gfx::GetAtom("_NET_WM_USER_TIME"),
                    XA_CARDINAL, 32, PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&wm_user_time_ms),
                    1);
  }

  UpdateMinAndMaxSize();

  if (window_properties_.empty()) {
    XDeleteProperty(xdisplay_, xwindow_, gfx::GetAtom("_NET_WM_STATE"));
  } else {
    ui::SetAtomArrayProperty(xwindow_, "_NET_WM_STATE", "ATOM",
                             std::vector<XAtom>(std::begin(window_properties_),
                                                std::end(window_properties_)));
  }

  XMapWindow(xdisplay_, xwindow_);
  window_mapped_in_client_ = true;
}
