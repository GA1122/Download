gfx::Size ConstrainedWidthView::CalculatePreferredSize() const {
  gfx::Size size = View::CalculatePreferredSize();
  if (size.width() <= max_width_)
    return size;
  return gfx::Size(max_width_, GetHeightForWidth(max_width_));
}
