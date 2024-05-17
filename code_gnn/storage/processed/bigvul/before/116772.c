void RenderViewTest::SendWebKeyboardEvent(
    const WebKit::WebKeyboardEvent& key_event) {
  scoped_ptr<IPC::Message> input_message(new ViewMsg_HandleInputEvent(0));
  input_message->WriteData(reinterpret_cast<const char*>(&key_event),
                           sizeof(WebKit::WebKeyboardEvent));
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(*input_message);
}
