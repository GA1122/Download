  void StartScroll(gfx::Point start) {
    timestamp_ = base::TimeTicks::Now();
    current_point_ = start;
    ui::GestureEvent event = ui::GestureEvent(
        current_point_.x(), current_point_.y(), ui::EF_NONE, timestamp_,
        ui::GestureEventDetails(ui::ET_GESTURE_SCROLL_BEGIN, 0, -1.0f));
    GetShelfLayoutManager()->ProcessGestureEvent(event);
  }
