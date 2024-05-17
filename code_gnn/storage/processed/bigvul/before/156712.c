void SimulateMouseClick(RenderWidgetHost* rwh, int x, int y) {
  blink::WebMouseEvent mouse_event(
      blink::WebInputEvent::kMouseDown, blink::WebInputEvent::kNoModifiers,
      blink::WebInputEvent::GetStaticTimeStampForTests());
  mouse_event.button = blink::WebPointerProperties::Button::kLeft;
  mouse_event.SetPositionInWidget(x, y);
  rwh->ForwardMouseEvent(mouse_event);
}
