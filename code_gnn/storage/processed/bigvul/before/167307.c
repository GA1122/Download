void ScrollableShelfView::ScrollByXOffset(float x_offset, bool animating) {
  ScrollToXOffset(scroll_offset_.x() + x_offset, animating);
}
