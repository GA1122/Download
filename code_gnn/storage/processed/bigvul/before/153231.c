gfx::Rect DesktopWindowTreeHostX11::GetRestoredBounds() const {
  if (!restored_bounds_in_pixels_.IsEmpty())
    return ToDIPRect(restored_bounds_in_pixels_);

  return GetWindowBoundsInScreen();
}
