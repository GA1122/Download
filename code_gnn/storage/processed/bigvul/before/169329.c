void SimulateGestureScrollSequence(WebContents* web_contents,
                                   const gfx::Point& point,
                                   const gfx::Vector2dF& delta) {
  RenderWidgetHostImpl* widget_host = RenderWidgetHostImpl::From(
      web_contents->GetRenderViewHost()->GetWidget());

  blink::WebGestureEvent scroll_begin(
      blink::WebGestureEvent::kGestureScrollBegin,
      blink::WebInputEvent::kNoModifiers, ui::EventTimeForNow(),
      blink::kWebGestureDeviceTouchpad);
  scroll_begin.SetPositionInWidget(gfx::PointF(point));
  scroll_begin.data.scroll_begin.delta_x_hint = delta.x();
  scroll_begin.data.scroll_begin.delta_y_hint = delta.y();
  widget_host->ForwardGestureEvent(scroll_begin);

  blink::WebGestureEvent scroll_update(
      blink::WebGestureEvent::kGestureScrollUpdate,
      blink::WebInputEvent::kNoModifiers, ui::EventTimeForNow(),
      blink::kWebGestureDeviceTouchpad);
  scroll_update.SetPositionInWidget(gfx::PointF(point));
  scroll_update.data.scroll_update.delta_x = delta.x();
  scroll_update.data.scroll_update.delta_y = delta.y();
  scroll_update.data.scroll_update.velocity_x = 0;
  scroll_update.data.scroll_update.velocity_y = 0;
  widget_host->ForwardGestureEvent(scroll_update);

  blink::WebGestureEvent scroll_end(blink::WebGestureEvent::kGestureScrollEnd,
                                    blink::WebInputEvent::kNoModifiers,
                                    ui::EventTimeForNow(),
                                    blink::kWebGestureDeviceTouchpad);
  scroll_end.SetPositionInWidget(gfx::PointF(point));
  widget_host->ForwardGestureEvent(scroll_end);
}
