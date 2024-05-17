void InputHandler::OnInputEvent(const blink::WebInputEvent& event) {
  input_queued_ = true;
}
