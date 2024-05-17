void InputHandler::OnInputEventAck(InputEventAckSource source,
                                   InputEventAckState state,
                                   const blink::WebInputEvent& event) {
  if ((event.GetModifiers() & blink::WebInputEvent::kFromDebugger) == 0)
    return;

  if (blink::WebInputEvent::IsKeyboardEventType(event.GetType()) &&
      !pending_key_callbacks_.empty()) {
    pending_key_callbacks_.front()->sendSuccess();
    pending_key_callbacks_.pop_front();
    return;
  }
  if ((blink::WebInputEvent::IsMouseEventType(event.GetType()) ||
       event.GetType() == blink::WebInputEvent::kMouseWheel) &&
      !pending_mouse_callbacks_.empty()) {
    pending_mouse_callbacks_.front()->sendSuccess();
    pending_mouse_callbacks_.pop_front();
    return;
  }
}
