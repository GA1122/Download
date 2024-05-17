void RenderViewTest::SendWebMouseEvent(
    const WebKit::WebMouseEvent& mouse_event) {
  scoped_ptr<IPC::Message> input_message(new ViewMsg_HandleInputEvent(0));
  input_message->WriteData(reinterpret_cast<const char*>(&mouse_event),
                           sizeof(WebKit::WebMouseEvent));
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(*input_message);
}
