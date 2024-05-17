void DesktopWindowTreeHostX11::MoveCursorToScreenLocationInPixels(
    const gfx::Point& location_in_pixels) {
  XWarpPointer(xdisplay_, x11::None, x_root_window_, 0, 0, 0, 0,
               bounds_in_pixels_.x() + location_in_pixels.x(),
               bounds_in_pixels_.y() + location_in_pixels.y());
}
