void DesktopWindowTreeHostX11::CenterWindow(const gfx::Size& size) {
  gfx::Size size_in_pixels = ToPixelRect(gfx::Rect(size)).size();
  gfx::Rect parent_bounds_in_pixels = ToPixelRect(GetWorkAreaBoundsInScreen());

  if (wm::GetTransientParent(content_window())) {
    gfx::Rect transient_parent_rect =
        wm::GetTransientParent(content_window())->GetBoundsInScreen();
    if (transient_parent_rect.height() >= size.height() &&
        transient_parent_rect.width() >= size.width()) {
      parent_bounds_in_pixels = ToPixelRect(transient_parent_rect);
    }
  }

  gfx::Rect window_bounds_in_pixels(
      parent_bounds_in_pixels.x() +
          (parent_bounds_in_pixels.width() - size_in_pixels.width()) / 2,
      parent_bounds_in_pixels.y() +
          (parent_bounds_in_pixels.height() - size_in_pixels.height()) / 2,
      size_in_pixels.width(), size_in_pixels.height());
  window_bounds_in_pixels.AdjustToFit(parent_bounds_in_pixels);

  SetBoundsInPixels(window_bounds_in_pixels);
}
