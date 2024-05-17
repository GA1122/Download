void RenderWidgetHostImpl::OnAutoscrollEnd() {
  WebGestureEvent cancel_event = SyntheticWebGestureEventBuilder::Build(
      WebInputEvent::kGestureFlingCancel,
      blink::kWebGestureDeviceSyntheticAutoscroll);
  cancel_event.data.fling_cancel.prevent_boosting = true;

  ForwardGestureEventWithLatencyInfo(
      cancel_event, ui::LatencyInfo(ui::SourceEventType::OTHER));
}
