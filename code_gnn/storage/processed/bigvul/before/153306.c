void DesktopWindowTreeHostX11::UpdateMinAndMaxSize() {
  gfx::Size minimum_in_pixels =
      ToPixelRect(gfx::Rect(native_widget_delegate_->GetMinimumSize())).size();
  gfx::Size maximum_in_pixels =
      ToPixelRect(gfx::Rect(native_widget_delegate_->GetMaximumSize())).size();
  if (min_size_in_pixels_ == minimum_in_pixels &&
      max_size_in_pixels_ == maximum_in_pixels)
    return;

  min_size_in_pixels_ = minimum_in_pixels;
  max_size_in_pixels_ = maximum_in_pixels;

  XSizeHints hints;
  hints.flags = 0;
  long supplied_return;
  XGetWMNormalHints(xdisplay_, xwindow_, &hints, &supplied_return);

  if (minimum_in_pixels.IsEmpty()) {
    hints.flags &= ~PMinSize;
  } else {
    hints.flags |= PMinSize;
    hints.min_width = min_size_in_pixels_.width();
    hints.min_height = min_size_in_pixels_.height();
  }

  if (maximum_in_pixels.IsEmpty()) {
    hints.flags &= ~PMaxSize;
  } else {
    hints.flags |= PMaxSize;
    hints.max_width = max_size_in_pixels_.width();
    hints.max_height = max_size_in_pixels_.height();
  }

  XSetWMNormalHints(xdisplay_, xwindow_, &hints);
}
