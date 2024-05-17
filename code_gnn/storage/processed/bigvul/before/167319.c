void ScrollableShelfView::UpdateLayoutStrategy(int available_length) {
  gfx::Size preferred_size = GetPreferredSize();
  int preferred_length = GetShelf()->IsHorizontalAlignment()
                             ? preferred_size.width()
                             : preferred_size.height();
  preferred_length += 2 * GetAppIconEndPadding();

  int scroll_length = GetShelf()->IsHorizontalAlignment() ? scroll_offset_.x()
                                                          : scroll_offset_.y();

  if (preferred_length <= available_length) {
    layout_strategy_ = kNotShowArrowButtons;
  } else if (scroll_length == 0) {
    layout_strategy_ = kShowRightArrowButton;
  } else if (scroll_length == CalculateScrollUpperBound()) {
    layout_strategy_ = kShowLeftArrowButton;
  } else {
    layout_strategy_ = kShowButtons;
  }
}
