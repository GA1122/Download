gfx::Rect DesktopWindowTreeHostX11::GetWindowBoundsInScreen() const {
  return ToDIPRect(bounds_in_pixels_);
}
