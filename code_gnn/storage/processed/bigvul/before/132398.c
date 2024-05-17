gfx::Rect FullWidthKeyboardBoundsFromRootBounds(const gfx::Rect& root_bounds,
                                                int keyboard_height) {
  return gfx::Rect(
      root_bounds.x(),
      root_bounds.bottom() - keyboard_height,
      root_bounds.width(),
      keyboard_height);
}
