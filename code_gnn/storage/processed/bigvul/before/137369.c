  void Tap(const gfx::Point& point) {
    GestureEventForTest begin(
        point.x(), point.y(), ui::GestureEventDetails(ui::ET_GESTURE_BEGIN));
    textfield_->OnGestureEvent(&begin);

    GestureEventForTest tap_down(
        point.x(), point.y(), ui::GestureEventDetails(ui::ET_GESTURE_TAP_DOWN));
    textfield_->OnGestureEvent(&tap_down);

    GestureEventForTest show_press(
        point.x(),
        point.y(),
        ui::GestureEventDetails(ui::ET_GESTURE_SHOW_PRESS));
    textfield_->OnGestureEvent(&show_press);

    ui::GestureEventDetails tap_details(ui::ET_GESTURE_TAP);
    tap_details.set_tap_count(1);
    GestureEventForTest tap(point.x(), point.y(), tap_details);
    textfield_->OnGestureEvent(&tap);

    GestureEventForTest end(
        point.x(), point.y(), ui::GestureEventDetails(ui::ET_GESTURE_END));
    textfield_->OnGestureEvent(&end);
  }
