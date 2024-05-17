void RenderViewTest::SendInputEvent(const blink::WebInputEvent& input_event) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(InputMsg_HandleInputEvent(
      0, &input_event, std::vector<const WebInputEvent*>(), ui::LatencyInfo(),
      InputEventDispatchType::DISPATCH_TYPE_BLOCKING));
}
