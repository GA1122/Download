void SimulateLongTapAt(WebContents* web_contents, const gfx::Point& point) {
  RenderWidgetHostViewAura* rwhva = static_cast<RenderWidgetHostViewAura*>(
      web_contents->GetRenderWidgetHostView());

  ui::TouchEvent touch_start(
      ui::ET_TOUCH_PRESSED, point, base::TimeTicks(),
      ui::PointerDetails(ui::EventPointerType::POINTER_TYPE_TOUCH, 0));
  rwhva->OnTouchEvent(&touch_start);

  ui::GestureEventDetails tap_down_details(ui::ET_GESTURE_TAP_DOWN);
  tap_down_details.set_device_type(ui::GestureDeviceType::DEVICE_TOUCHSCREEN);
  ui::GestureEvent tap_down(point.x(), point.y(), 0, ui::EventTimeForNow(),
                            tap_down_details, touch_start.unique_event_id());
  rwhva->OnGestureEvent(&tap_down);

  ui::GestureEventDetails long_press_details(ui::ET_GESTURE_LONG_PRESS);
  long_press_details.set_device_type(ui::GestureDeviceType::DEVICE_TOUCHSCREEN);
  ui::GestureEvent long_press(point.x(), point.y(), 0, ui::EventTimeForNow(),
                              long_press_details,
                              touch_start.unique_event_id());
  rwhva->OnGestureEvent(&long_press);

  ui::TouchEvent touch_end(
      ui::ET_TOUCH_RELEASED, point, base::TimeTicks(),
      ui::PointerDetails(ui::EventPointerType::POINTER_TYPE_TOUCH, 0));
  rwhva->OnTouchEvent(&touch_end);

  ui::GestureEventDetails long_tap_details(ui::ET_GESTURE_LONG_TAP);
  long_tap_details.set_device_type(ui::GestureDeviceType::DEVICE_TOUCHSCREEN);
  ui::GestureEvent long_tap(point.x(), point.y(), 0, ui::EventTimeForNow(),
                            long_tap_details, touch_end.unique_event_id());
  rwhva->OnGestureEvent(&long_tap);
}
