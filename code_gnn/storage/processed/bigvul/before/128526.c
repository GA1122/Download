PP_CursorType_Dev Instance::CalculateAutoscroll(const pp::Point& mouse_pos) {
  if (autoscroll_rect_.Contains(mouse_pos)) {
    autoscroll_x_ = 0;
    autoscroll_y_ = 0;
    return PP_CURSORTYPE_MIDDLEPANNING;
  }

  pp::Point rel_pos = mouse_pos - autoscroll_rect_.CenterPoint();

  double angle = atan2(static_cast<double>(rel_pos.y()),
                       static_cast<double>(rel_pos.x()));

  autoscroll_x_ = rel_pos.x() * kAutoScrollFactor;
  autoscroll_y_ = rel_pos.y() * kAutoScrollFactor;

  if (angle < - (M_PI * 7.0 / 8.0)) {
    return PP_CURSORTYPE_WESTPANNING;
  } else if (angle < - (M_PI * 5.0 / 8.0)) {
    return PP_CURSORTYPE_NORTHWESTPANNING;
  } else if (angle < - (M_PI * 3.0 / 8.0)) {
    return PP_CURSORTYPE_NORTHPANNING;
  } else if (angle < - (M_PI * 1.0 / 8.0)) {
    return PP_CURSORTYPE_NORTHEASTPANNING;
  } else if (angle < M_PI * 1.0 / 8.0) {
    return PP_CURSORTYPE_EASTPANNING;
  } else if (angle < M_PI * 3.0 / 8.0) {
    return PP_CURSORTYPE_SOUTHEASTPANNING;
  } else if (angle < M_PI * 5.0 / 8.0) {
    return PP_CURSORTYPE_SOUTHPANNING;
  } else if (angle < M_PI * 7.0 / 8.0) {
    return PP_CURSORTYPE_SOUTHWESTPANNING;
  }

  return PP_CURSORTYPE_WESTPANNING;
}
