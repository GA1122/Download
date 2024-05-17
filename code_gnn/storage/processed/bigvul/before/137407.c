void RenderViewTest::SendWebKeyboardEvent(
    const blink::WebKeyboardEvent& key_event) {
  SendInputEvent(key_event);
}
