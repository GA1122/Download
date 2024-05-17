ui::EventResult RenderWidgetHostViewAura::OnScrollEvent(
    ui::ScrollEvent* event) {
  TRACE_EVENT0("browser", "RenderWidgetHostViewAura::OnScrollEvent");
  if (event->type() == ui::ET_SCROLL) {
    WebKit::WebGestureEvent gesture_event =
        MakeWebGestureEventFlingCancel();
    host_->ForwardGestureEvent(gesture_event);
    WebKit::WebMouseWheelEvent mouse_wheel_event =
        MakeWebMouseWheelEvent(static_cast<ui::ScrollEvent*>(event));
    host_->ForwardWheelEvent(mouse_wheel_event);
    RecordAction(UserMetricsAction("TrackpadScroll"));
  } else if (event->type() == ui::ET_SCROLL_FLING_START ||
      event->type() == ui::ET_SCROLL_FLING_CANCEL) {
    WebKit::WebGestureEvent gesture_event =
        MakeWebGestureEvent(static_cast<ui::ScrollEvent*>(event));
    host_->ForwardGestureEvent(gesture_event);
    if (event->type() == ui::ET_SCROLL_FLING_START)
      RecordAction(UserMetricsAction("TrackpadScrollFling"));
  }
  return ui::ER_HANDLED;
}
