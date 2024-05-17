bool DesktopWindowTreeHostX11::IsMinimized() const {
  return ui::HasWMSpecProperty(window_properties_,
                               gfx::GetAtom("_NET_WM_STATE_HIDDEN"));
}
