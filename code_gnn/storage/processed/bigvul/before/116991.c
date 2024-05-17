void GesturePoint::UpdateValues(const TouchEvent& event) {
  const int64 event_timestamp_microseconds =
      event.time_stamp().InMicroseconds();
  if (event.type() == ui::ET_TOUCH_MOVED) {
    velocity_calculator_.PointSeen(event.x(),
                                   event.y(),
                                   event_timestamp_microseconds);
  }

  last_touch_time_ = event.time_stamp().InSecondsF();
  last_touch_position_ = event.location();

  if (event.type() == ui::ET_TOUCH_PRESSED) {
    first_touch_time_ = last_touch_time_;
    first_touch_position_ = event.location();
    velocity_calculator_.ClearHistory();
    velocity_calculator_.PointSeen(event.x(),
                                   event.y(),
                                   event_timestamp_microseconds);
  }
}
