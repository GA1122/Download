void SimulateMouseEvent(WebContents* web_contents,
                        blink::WebInputEvent::Type type,
                        const gfx::Point& point) {
  blink::WebMouseEvent mouse_event(type, blink::WebInputEvent::kNoModifiers,
                                   ui::EventTimeForNow());
  mouse_event.SetPositionInWidget(point.x(), point.y());
  web_contents->GetRenderViewHost()->GetWidget()->ForwardMouseEvent(
      mouse_event);
}
