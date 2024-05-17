void RenderWidgetHostImpl::SimulateTouchGestureWithMouse(
    const WebMouseEvent& mouse_event) {
  int x = mouse_event.x, y = mouse_event.y;
  float dx = mouse_event.movementX, dy = mouse_event.movementY;
  static int startX = 0, startY = 0;

  switch (mouse_event.button) {
    case WebMouseEvent::ButtonLeft:
      if (mouse_event.type == WebInputEvent::MouseDown) {
        startX = x;
        startY = y;
        ForwardGestureEvent(MakeGestureEvent(
            WebInputEvent::GestureScrollBegin, mouse_event.timeStampSeconds,
            x, y, 0));
      }
      if (dx != 0 || dy != 0) {
        WebGestureEvent event = MakeGestureEvent(
            WebInputEvent::GestureScrollUpdate, mouse_event.timeStampSeconds,
            x, y, 0);
        event.data.scrollUpdate.deltaX = dx;
        event.data.scrollUpdate.deltaY = dy;
        ForwardGestureEvent(event);
      }
      if (mouse_event.type == WebInputEvent::MouseUp) {
        ForwardGestureEvent(MakeGestureEvent(
            WebInputEvent::GestureScrollEnd, mouse_event.timeStampSeconds,
            x, y, 0));
      }
      break;
    case WebMouseEvent::ButtonMiddle:
      if (mouse_event.type == WebInputEvent::MouseDown) {
        startX = x;
        startY = y;
        ForwardGestureEvent(MakeGestureEvent(
            WebInputEvent::GestureTapDown, mouse_event.timeStampSeconds,
            x, y, 0));
      }
      if (mouse_event.type == WebInputEvent::MouseUp) {
        ForwardGestureEvent(MakeGestureEvent(
            WebInputEvent::GestureTap, mouse_event.timeStampSeconds,
            x, y, 0));
      }
      break;
    case WebMouseEvent::ButtonRight:
      if (mouse_event.type == WebInputEvent::MouseDown) {
        startX = x;
        startY = y;
        ForwardGestureEvent(MakeGestureEvent(
            WebInputEvent::GesturePinchBegin, mouse_event.timeStampSeconds,
            x, y, 0));
      }
      if (dx != 0 || dy != 0) {
        dx = pow(dy < 0 ? 0.998f : 1.002f, fabs(dy));
        WebGestureEvent event = MakeGestureEvent(
            WebInputEvent::GesturePinchUpdate, mouse_event.timeStampSeconds,
            startX, startY, 0);
        event.data.pinchUpdate.scale = dx;
        ForwardGestureEvent(event);
      }
      if (mouse_event.type == WebInputEvent::MouseUp) {
        ForwardGestureEvent(MakeGestureEvent(
            WebInputEvent::GesturePinchEnd, mouse_event.timeStampSeconds,
            x, y, 0));
      }
      break;
    case WebMouseEvent::ButtonNone:
      break;
  }
}
