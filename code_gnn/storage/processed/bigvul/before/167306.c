bool ScrollableShelfView::ProcessGestureEvent(const ui::GestureEvent& event) {
  if (layout_strategy_ == kNotShowArrowButtons)
    return true;

  if (event.type() == ui::ET_GESTURE_SCROLL_BEGIN ||
      event.type() == ui::ET_GESTURE_SCROLL_END) {
    return true;
  }

  if (event.type() == ui::ET_GESTURE_END) {
    scroll_offset_ = gfx::ToFlooredVector2d(scroll_offset_);

    int actual_scroll_distance = GetActualScrollOffset();

    if (actual_scroll_distance == CalculateScrollUpperBound())
      return true;

    const int residue = actual_scroll_distance % GetUnit();
    int offset =
        residue > GetGestureDragThreshold() ? GetUnit() - residue : -residue;

    if (!offset)
      return true;

    if (GetShelf()->IsHorizontalAlignment())
      ScrollByXOffset(offset,  true);
    else
      ScrollByYOffset(offset,  true);
    return true;
  }

  if (event.type() == ui::ET_SCROLL_FLING_START) {
    const bool is_horizontal_alignment = GetShelf()->IsHorizontalAlignment();

    int scroll_velocity = is_horizontal_alignment
                              ? event.details().velocity_x()
                              : event.details().velocity_y();
    if (abs(scroll_velocity) < kFlingVelocityThreshold)
      return false;

    layout_strategy_ = layout_strategy_before_main_axis_scrolling_;

    float page_scrolling_offset =
        CalculatePageScrollingOffset(scroll_velocity < 0);
    if (is_horizontal_alignment) {
      ScrollToXOffset(
          scroll_offset_before_main_axis_scrolling_.x() + page_scrolling_offset,
          true);
    } else {
      ScrollToYOffset(
          scroll_offset_before_main_axis_scrolling_.y() + page_scrolling_offset,
          true);
    }

    return true;
  }

  if (event.type() != ui::ET_GESTURE_SCROLL_UPDATE)
    return false;

  if (GetShelf()->IsHorizontalAlignment())
    ScrollByXOffset(-event.details().scroll_x(),  false);
  else
    ScrollByYOffset(-event.details().scroll_y(),  false);
  return true;
}
