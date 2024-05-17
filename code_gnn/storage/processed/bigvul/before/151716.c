bool Browser::PreHandleGestureEvent(content::WebContents* source,
                                    const blink::WebGestureEvent& event) {
  if (app_name() == DevToolsWindow::kDevToolsApp)
    return event.type == blink::WebGestureEvent::GesturePinchBegin ||
           event.type == blink::WebGestureEvent::GesturePinchUpdate ||
           event.type == blink::WebGestureEvent::GesturePinchEnd;

  return false;
}
