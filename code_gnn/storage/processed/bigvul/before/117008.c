GestureSequence::Gestures* GestureSequence::ProcessTouchEventForGesture(
    const TouchEvent& event,
    ui::TouchStatus status) {
  if (status != ui::TOUCH_STATUS_UNKNOWN)
    return NULL;   

  if (event.touch_id() >= kMaxGesturePoints)
    return NULL;

  if (event.type() == ui::ET_TOUCH_PRESSED) {
    if (point_count_ == kMaxGesturePoints)
      return NULL;
    ++point_count_;
  }

  GestureState last_state = state_;

  scoped_ptr<Gestures> gestures(new Gestures());
  GesturePoint& point = GesturePointForEvent(event);
  point.UpdateValues(event);
  flags_ = event.flags();
  switch (Signature(state_, event.touch_id(), event.type(), false)) {
    case GST_NO_GESTURE_FIRST_PRESSED:
      TouchDown(event, point, gestures.get());
      set_state(GS_PENDING_SYNTHETIC_CLICK);
      break;
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_RELEASED:
      if (Click(event, point, gestures.get()))
        point.UpdateForTap();
      set_state(GS_NO_GESTURE);
      break;
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_MOVED:
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_STATIONARY:
      if (ScrollStart(event, point, gestures.get())) {
        set_state(GS_SCROLL);
        if (ScrollUpdate(event, point, gestures.get()))
          point.UpdateForScroll();
      }
      break;
    case GST_PENDING_SYNTHETIC_CLICK_FIRST_CANCELLED:
      NoGesture(event, point, gestures.get());
      break;
    case GST_SCROLL_FIRST_MOVED:
    case GST_SCROLL_SECOND_MOVED:
      if (scroll_type_ == ST_VERTICAL ||
          scroll_type_ == ST_HORIZONTAL)
        BreakRailScroll(event, point, gestures.get());
      if (ScrollUpdate(event, point, gestures.get()))
        point.UpdateForScroll();
      break;
    case GST_SCROLL_FIRST_RELEASED:
    case GST_SCROLL_FIRST_CANCELLED:
    case GST_SCROLL_SECOND_RELEASED:
    case GST_SCROLL_SECOND_CANCELLED:
      ScrollEnd(event, point, gestures.get());
      set_state(GS_NO_GESTURE);
      break;
    case GST_SCROLL_FIRST_PRESSED:
    case GST_SCROLL_SECOND_PRESSED:
    case GST_PENDING_SYNTHETIC_CLICK_SECOND_PRESSED:
      PinchStart(event, point, gestures.get());
      set_state(GS_PINCH);
      break;
    case GST_PINCH_FIRST_MOVED:
    case GST_PINCH_SECOND_MOVED:
      if (PinchUpdate(event, point, gestures.get())) {
        points_[0].UpdateForScroll();
        points_[1].UpdateForScroll();
      }
      break;
    case GST_PINCH_FIRST_RELEASED:
    case GST_PINCH_SECOND_RELEASED:
    case GST_PINCH_FIRST_CANCELLED:
    case GST_PINCH_SECOND_CANCELLED:
      PinchEnd(event, point, gestures.get());

      scroll_type_ = ST_FREE;
      set_state(GS_SCROLL);
      break;
  }

  if (state_ != last_state)
    VLOG(4) << "Gesture Sequence"
            << " State: " << state_
            << " touch id: " << event.touch_id();

  if (event.type() == ui::ET_TOUCH_RELEASED)
    --point_count_;

  return gestures.release();
}
