void SimulateMouseWheelCtrlZoomEvent(WebContents* web_contents,
                                     const gfx::Point& point,
                                     bool zoom_in,
                                     blink::WebMouseWheelEvent::Phase phase) {
  blink::WebMouseWheelEvent wheel_event(blink::WebInputEvent::kMouseWheel,
                                        blink::WebInputEvent::kControlKey,
                                        ui::EventTimeForNow());

  wheel_event.SetPositionInWidget(point.x(), point.y());
  wheel_event.delta_y =
      (zoom_in ? 1.0 : -1.0) * ui::MouseWheelEvent::kWheelDelta;
  wheel_event.wheel_ticks_y = (zoom_in ? 1.0 : -1.0);
  wheel_event.has_precise_scrolling_deltas = false;
  wheel_event.phase = phase;
  RenderWidgetHostImpl* widget_host = RenderWidgetHostImpl::From(
      web_contents->GetRenderViewHost()->GetWidget());
  widget_host->ForwardWheelEvent(wheel_event);
}
