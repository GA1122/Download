bool PanelBrowserView::OnTitlebarMousePressed(
    const gfx::Point& mouse_location) {
  mouse_pressed_ = true;
  mouse_dragging_state_ = NO_DRAGGING;
  last_mouse_location_ = mouse_location;
  return true;
}
