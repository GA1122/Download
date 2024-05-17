bool RenderViewTest::SimulateElementClick(const std::string& element_id) {
  gfx::Rect bounds = GetElementBounds(element_id);
  if (bounds.IsEmpty())
    return false;
  WebMouseEvent mouse_event;
  mouse_event.type = WebInputEvent::MouseDown;
  mouse_event.button = WebMouseEvent::ButtonLeft;
  mouse_event.x = bounds.CenterPoint().x();
  mouse_event.y = bounds.CenterPoint().y();
  mouse_event.clickCount = 1;
  ViewMsg_HandleInputEvent input_event(0);
  scoped_ptr<IPC::Message> input_message(new ViewMsg_HandleInputEvent(0));
  input_message->WriteData(reinterpret_cast<const char*>(&mouse_event),
                           sizeof(WebMouseEvent));
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(*input_message);
  return true;
}
