void RenderViewTest::SimulatePointClick(const gfx::Point& point) {
  WebMouseEvent mouse_event;
  mouse_event.type = WebInputEvent::MouseDown;
  mouse_event.button = WebMouseEvent::ButtonLeft;
  mouse_event.x = point.x();
  mouse_event.y = point.y();
  mouse_event.clickCount = 1;
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(
      InputMsg_HandleInputEvent(0, &mouse_event, ui::LatencyInfo(), false));
  mouse_event.type = WebInputEvent::MouseUp;
  impl->OnMessageReceived(
      InputMsg_HandleInputEvent(0, &mouse_event, ui::LatencyInfo(), false));
}
