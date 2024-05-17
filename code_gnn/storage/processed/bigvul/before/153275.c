void DesktopWindowTreeHostX11::Restore() {
  should_maximize_after_map_ = false;
  SetWMSpecState(false, gfx::GetAtom("_NET_WM_STATE_MAXIMIZED_VERT"),
                 gfx::GetAtom("_NET_WM_STATE_MAXIMIZED_HORZ"));
  Show(ui::SHOW_STATE_NORMAL, gfx::Rect());
  SetWMSpecState(false, gfx::GetAtom("_NET_WM_STATE_HIDDEN"), x11::None);
}
