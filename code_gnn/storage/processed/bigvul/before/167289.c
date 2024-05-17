views::View* ScrollableShelfView::GetDefaultFocusableChild() {

  if (default_last_focusable_child_) {
    scroll_offset_ = GetShelf()->IsHorizontalAlignment()
                         ? gfx::Vector2dF(CalculateScrollUpperBound(), 0)
                         : gfx::Vector2dF(0, CalculateScrollUpperBound());
    Layout();
    return FindLastFocusableChild();
  } else {
    scroll_offset_ = gfx::Vector2dF();
    Layout();
    return FindFirstFocusableChild();
  }
}
