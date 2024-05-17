void RenderViewTest::SimulateRectTap(const gfx::Rect& rect) {
  WebGestureEvent gesture_event;
  gesture_event.x = rect.CenterPoint().x();
  gesture_event.y = rect.CenterPoint().y();
  gesture_event.data.tap.tapCount = 1;
  gesture_event.data.tap.width = rect.width();
  gesture_event.data.tap.height = rect.height();
  gesture_event.type = WebInputEvent::GestureTap;
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(
      InputMsg_HandleInputEvent(0, &gesture_event, ui::LatencyInfo(), false));
  impl->FocusChangeComplete();
}
