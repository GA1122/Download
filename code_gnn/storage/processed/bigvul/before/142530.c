  void UpdateScroll(float delta_y) {
    IncreaseTimestamp();
    current_point_.set_y(current_point_.y() + delta_y);
    ui::GestureEvent event = ui::GestureEvent(
        current_point_.x(), current_point_.y(), ui::EF_NONE, timestamp_,
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_UPDATE, 0, delta_y));
    GetShelfLayoutManager()->ProcessGestureEvent(event);
  }
