void DesktopWindowTreeHostX11::Activate() {
  if (!IsVisible() || !activatable_)
    return;

  BeforeActivationStateChanged();

  ignore_keyboard_input_ = false;

  static bool wm_supports_active_window =
      ui::GuessWindowManager() != ui::WM_WMII &&
      ui::WmSupportsHint(gfx::GetAtom("_NET_ACTIVE_WINDOW"));

  Time timestamp = ui::X11EventSource::GetInstance()->GetTimestamp();

  if (wm_supports_active_window && !override_redirect_) {
    XEvent xclient;
    memset(&xclient, 0, sizeof(xclient));
    xclient.type = ClientMessage;
    xclient.xclient.window = xwindow_;
    xclient.xclient.message_type = gfx::GetAtom("_NET_ACTIVE_WINDOW");
    xclient.xclient.format = 32;
    xclient.xclient.data.l[0] = 1;   
    xclient.xclient.data.l[1] = timestamp;
    xclient.xclient.data.l[2] = x11::None;
    xclient.xclient.data.l[3] = 0;
    xclient.xclient.data.l[4] = 0;

    XSendEvent(xdisplay_, x_root_window_, x11::False,
               SubstructureRedirectMask | SubstructureNotifyMask, &xclient);
  } else {
    XRaiseWindow(xdisplay_, xwindow_);
    auto old_error_handler = XSetErrorHandler(IgnoreX11Errors);
    XSetInputFocus(xdisplay_, xwindow_, RevertToParent, timestamp);
    has_pointer_focus_ = false;
    has_window_focus_ = true;
    window_mapped_in_server_ = true;
    XSetErrorHandler(old_error_handler);
  }
  AfterActivationStateChanged();
}
