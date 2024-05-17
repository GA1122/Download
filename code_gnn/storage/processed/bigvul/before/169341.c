void SimulateMouseWheelEvent(WebContents* web_contents,
                             const gfx::Point& point,
                             const gfx::Vector2d& delta,
                             const blink::WebMouseWheelEvent::Phase phase) {
  blink::WebMouseWheelEvent wheel_event(blink::WebInputEvent::kMouseWheel,
                                        blink::WebInputEvent::kNoModifiers,
                                        ui::EventTimeForNow());

  wheel_event.SetPositionInWidget(point.x(), point.y());
  wheel_event.delta_x = delta.x();
  wheel_event.delta_y = delta.y();
  wheel_event.phase = phase;
  RenderWidgetHostImpl* widget_host = RenderWidgetHostImpl::From(
      web_contents->GetRenderViewHost()->GetWidget());
  widget_host->ForwardWheelEvent(wheel_event);
}
