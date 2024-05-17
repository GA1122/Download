gfx::Rect DesktopWindowTreeHostX11::GetX11RootWindowOuterBounds() const {
  gfx::Rect outer_bounds(bounds_in_pixels_);
  outer_bounds.Inset(-native_window_frame_borders_in_pixels_);
  return outer_bounds;
}
