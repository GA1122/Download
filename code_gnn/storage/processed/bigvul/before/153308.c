void DesktopWindowTreeHostX11::UpdateWindowProperties(
    const base::flat_set<XAtom>& new_window_properties) {
  bool was_minimized = IsMinimized();

  window_properties_ = new_window_properties;

  bool is_minimized = IsMinimized();

  if (is_minimized != was_minimized) {
    if (is_minimized) {
      SetVisible(false);
      content_window()->Hide();
    } else {
      content_window()->Show();
      SetVisible(true);
    }
  }

  if (restored_bounds_in_pixels_.IsEmpty()) {
    if (IsMaximized()) {
      restored_bounds_in_pixels_ = previous_bounds_in_pixels_;
    }
  } else if (!IsMaximized() && !IsFullscreen()) {
    restored_bounds_in_pixels_ = gfx::Rect();
  }


  is_always_on_top_ = ui::HasWMSpecProperty(
      window_properties_, gfx::GetAtom("_NET_WM_STATE_ABOVE"));

  Relayout();
  ResetWindowRegion();
}
