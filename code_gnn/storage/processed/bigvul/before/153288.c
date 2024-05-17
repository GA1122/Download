 void DesktopWindowTreeHostX11::SetVisibleOnAllWorkspaces(bool always_visible) {
  SetWMSpecState(always_visible, gfx::GetAtom("_NET_WM_STATE_STICKY"),
                 x11::None);

  int new_desktop = 0;
  if (always_visible) {
    new_desktop = kAllDesktops;
  } else {
    if (!ui::GetCurrentDesktop(&new_desktop))
      return;
  }

  workspace_ = kAllDesktops;
  XEvent xevent;
  memset (&xevent, 0, sizeof (xevent));
  xevent.type = ClientMessage;
  xevent.xclient.window = xwindow_;
  xevent.xclient.message_type = gfx::GetAtom("_NET_WM_DESKTOP");
  xevent.xclient.format = 32;
  xevent.xclient.data.l[0] = new_desktop;
  xevent.xclient.data.l[1] = 0;
  xevent.xclient.data.l[2] = 0;
  xevent.xclient.data.l[3] = 0;
  xevent.xclient.data.l[4] = 0;
  XSendEvent(xdisplay_, x_root_window_, x11::False,
             SubstructureRedirectMask | SubstructureNotifyMask, &xevent);
}
