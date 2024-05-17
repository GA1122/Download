void SimulateMouseClickAt(WebContents* web_contents,
                          int modifiers,
                          blink::WebMouseEvent::Button button,
                          const gfx::Point& point) {
  blink::WebMouseEvent mouse_event(blink::WebInputEvent::kMouseDown, modifiers,
                                   ui::EventTimeForNow());
  mouse_event.button = button;
  mouse_event.SetPositionInWidget(point.x(), point.y());
  gfx::Rect offset = web_contents->GetContainerBounds();
  mouse_event.SetPositionInScreen(point.x() + offset.x(),
                                  point.y() + offset.y());
  mouse_event.click_count = 1;
  web_contents->GetRenderViewHost()->GetWidget()->ForwardMouseEvent(
      mouse_event);
  mouse_event.SetType(blink::WebInputEvent::kMouseUp);
  web_contents->GetRenderViewHost()->GetWidget()->ForwardMouseEvent(
      mouse_event);
}
