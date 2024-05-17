EdgeStateSignatureType Signature(GestureState gesture_state,
                                 unsigned int touch_id,
                                 ui::EventType type,
                                 bool touch_handled) {
  CHECK((touch_id & 0xfff) == touch_id);
  TouchState touch_state = TouchEventTypeToTouchState(type);
  return static_cast<EdgeStateSignatureType>
      (G(gesture_state, touch_id, touch_state, touch_handled));
}
