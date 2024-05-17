float ScrollableShelfView::CalculateClampedScrollOffset(float scroll) const {
  const float scroll_upper_bound = CalculateScrollUpperBound();
  scroll = base::ClampToRange(scroll, 0.0f, scroll_upper_bound);
  return scroll;
}
