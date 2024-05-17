void PerformGesture(CustomButton* button, ui::EventType event_type) {
  ui::GestureEventDetails gesture_details(event_type);
  base::TimeDelta time_stamp = base::TimeDelta::FromMicroseconds(0);
  ui::GestureEvent gesture_event(0, 0, 0, time_stamp, gesture_details);
  button->OnGestureEvent(&gesture_event);
}
