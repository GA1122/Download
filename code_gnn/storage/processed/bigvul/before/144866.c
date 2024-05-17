  void HandleMouseLeaveEvent() {
    WebMouseEvent event(
        WebInputEvent::kMouseMove, WebFloatPoint(1, 1), WebFloatPoint(1, 1),
        WebPointerProperties::Button::kLeft, 0,
        WebInputEvent::Modifiers::kLeftButtonDown, CurrentTimeTicksInSeconds());
    event.SetFrameScale(1);
    GetEventHandler().HandleMouseLeaveEvent(event);
  }
