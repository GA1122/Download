InputEventAckState SynchronousCompositorImpl::HandleInputEvent(
    const blink::WebInputEvent& input_event) {
  DCHECK(CalledOnValidThread());
  return g_factory.Get().synchronous_input_event_filter()->HandleInputEvent(
      routing_id_, input_event);
}
