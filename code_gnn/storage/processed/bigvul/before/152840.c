void MetricsWebContentsObserver::OnInputEvent(
    const blink::WebInputEvent& event) {
  if (event.GetType() == blink::WebInputEvent::Type::kUndefined)
    return;

  if (committed_load_)
    committed_load_->OnInputEvent(event);
}
