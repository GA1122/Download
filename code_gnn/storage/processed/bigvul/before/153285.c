void DesktopWindowTreeHostX11::SetSize(const gfx::Size& requested_size) {
  gfx::Size size_in_pixels = ToPixelRect(gfx::Rect(requested_size)).size();
  size_in_pixels = AdjustSize(size_in_pixels);
  bool size_changed = bounds_in_pixels_.size() != size_in_pixels;
  XResizeWindow(xdisplay_, xwindow_, size_in_pixels.width(),
                size_in_pixels.height());
  bounds_in_pixels_.set_size(size_in_pixels);
  if (size_changed) {
    OnHostResizedInPixels(size_in_pixels);
    ResetWindowRegion();
  }
}
