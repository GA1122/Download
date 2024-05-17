ui::EventResult RenderWidgetHostViewAura::OnTouchEvent(ui::TouchEvent* event) {
  TRACE_EVENT0("browser", "RenderWidgetHostViewAura::OnTouchEvent");
  WebKit::WebTouchPoint* point = UpdateWebTouchEventFromUIEvent(*event,
                                                                &touch_event_);

  ui::EventResult result = host_->ShouldForwardTouchEvent() ? ui::ER_CONSUMED :
                                                              ui::ER_UNHANDLED;
  if (point) {
    if (host_->ShouldForwardTouchEvent())
      host_->ForwardTouchEvent(touch_event_);
    UpdateWebTouchEventAfterDispatch(&touch_event_, point);
  }

  return result;
}
