bool DesktopWindowTreeHostX11::IsMaximized() const {
  return (ui::HasWMSpecProperty(window_properties_,
                                gfx::GetAtom("_NET_WM_STATE_MAXIMIZED_VERT")) &&
          ui::HasWMSpecProperty(window_properties_,
                                gfx::GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ")));
}
