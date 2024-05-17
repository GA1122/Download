 bool ScrollableShelfView::ShouldHandleGestures(const ui::GestureEvent& event) {

  if (scroll_status_ == kNotInScroll && !event.IsScrollGestureEvent())
    return false;

  if (event.type() == ui::ET_GESTURE_SCROLL_BEGIN) {
    CHECK_EQ(scroll_status_, kNotInScroll);

    float main_offset = event.details().scroll_x_hint();
    float cross_offset = event.details().scroll_y_hint();
    if (!GetShelf()->IsHorizontalAlignment())
      std::swap(main_offset, cross_offset);

    scroll_status_ = std::abs(main_offset) < std::abs(cross_offset)
                         ? kAcrossMainAxisScroll
                         : kAlongMainAxisScroll;
  }

  bool should_handle_gestures = scroll_status_ == kAlongMainAxisScroll;

  if (scroll_status_ == kAlongMainAxisScroll &&
      event.type() == ui::ET_GESTURE_SCROLL_BEGIN) {
    scroll_offset_before_main_axis_scrolling_ = scroll_offset_;
    layout_strategy_before_main_axis_scrolling_ = layout_strategy_;
  }

  if (event.type() == ui::ET_GESTURE_END) {
    scroll_status_ = kNotInScroll;

    if (should_handle_gestures) {
      scroll_offset_before_main_axis_scrolling_ = gfx::Vector2dF();
      layout_strategy_before_main_axis_scrolling_ = kNotShowArrowButtons;
    }
  }

  return should_handle_gestures;
}
