void DesktopWindowTreeHostX11::Minimize() {
  ReleaseCapture();
  if (window_mapped_in_client_)
    XIconifyWindow(xdisplay_, xwindow_, 0);
  else
    SetWMSpecState(true, gfx::GetAtom("_NET_WM_STATE_HIDDEN"), x11::None);
}
