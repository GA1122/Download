void RenderViewTest::SimulateRectTap(const gfx::Rect& rect) {
  WebGestureEvent gesture_event(
      WebInputEvent::kGestureTap, WebInputEvent::kNoModifiers,
      ui::EventTimeStampToSeconds(ui::EventTimeForNow()));
  gesture_event.x = rect.CenterPoint().x();
  gesture_event.y = rect.CenterPoint().y();
  gesture_event.data.tap.tap_count = 1;
  gesture_event.data.tap.width = rect.width();
  gesture_event.data.tap.height = rect.height();
  gesture_event.source_device = blink::kWebGestureDeviceTouchpad;
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(InputMsg_HandleInputEvent(
      0, &gesture_event, std::vector<const WebInputEvent*>(), ui::LatencyInfo(),
      InputEventDispatchType::DISPATCH_TYPE_BLOCKING));
  impl->FocusChangeComplete();
}
