void RenderWidgetHostImpl::OnAutoscrollStart(const gfx::PointF& position) {
  GetView()->OnAutoscrollStart();
  WebGestureEvent scroll_begin = SyntheticWebGestureEventBuilder::Build(
      WebInputEvent::kGestureScrollBegin,
      blink::kWebGestureDeviceSyntheticAutoscroll);
  scroll_begin.SetPositionInWidget(position);

  ForwardGestureEventWithLatencyInfo(
      scroll_begin, ui::LatencyInfo(ui::SourceEventType::OTHER));

  OnAutoscrollFling(gfx::Vector2dF());
}
