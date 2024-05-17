void DesktopWindowTreeHostX11::DelayedResize(const gfx::Size& size_in_pixels) {
  OnHostResizedInPixels(size_in_pixels);
  ResetWindowRegion();
  delayed_resize_task_.Cancel();
}
