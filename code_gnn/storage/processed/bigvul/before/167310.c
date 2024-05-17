void ScrollableShelfView::ScrollToXOffset(float x_target_offset,
                                          bool animating) {
  x_target_offset = CalculateClampedScrollOffset(x_target_offset);
  const float old_x = scroll_offset_.x();
  scroll_offset_.set_x(x_target_offset);
  Layout();
  const float diff = x_target_offset - old_x;

  if (animating)
    StartShelfScrollAnimation(diff);
}
