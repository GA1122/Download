void RenderWidgetHostImpl::ForwardGestureEvent(
    const blink::WebGestureEvent& gesture_event) {
  ForwardGestureEventWithLatencyInfo(
      gesture_event,
      ui::WebInputEventTraits::CreateLatencyInfoForWebGestureEvent(
          gesture_event));
}
