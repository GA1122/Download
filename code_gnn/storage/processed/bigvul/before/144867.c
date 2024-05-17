  void HandleMouseMoveEvent(int x, int y) {
    WebMouseEvent event(
        WebInputEvent::kMouseMove, WebFloatPoint(x, y), WebFloatPoint(x, y),
        WebPointerProperties::Button::kNoButton, 0, WebInputEvent::kNoModifiers,
        CurrentTimeTicksInSeconds());
    event.SetFrameScale(1);
    GetEventHandler().HandleMouseMoveEvent(event, Vector<WebMouseEvent>());
  }
