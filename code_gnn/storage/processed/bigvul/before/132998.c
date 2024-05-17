void RenderWidgetHostViewAura::OnTouchEvent(ui::TouchEvent* event) {
  TRACE_EVENT0("input", "RenderWidgetHostViewAura::OnTouchEvent");
  if (touch_editing_client_ && touch_editing_client_->HandleInputEvent(event))
    return;

  blink::WebTouchPoint* point = UpdateWebTouchEventFromUIEvent(*event,
                                                                &touch_event_);

  if (host_->ShouldForwardTouchEvent())
    event->StopPropagation();

  if (point) {
    if (host_->ShouldForwardTouchEvent())
      host_->ForwardTouchEventWithLatencyInfo(touch_event_, *event->latency());
    UpdateWebTouchEventAfterDispatch(&touch_event_, point);
  }
}
