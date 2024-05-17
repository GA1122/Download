void RenderViewTest::SendWebKeyboardEvent(
    const blink::WebKeyboardEvent& key_event) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(
      InputMsg_HandleInputEvent(0, &key_event, ui::LatencyInfo(), false));
}
