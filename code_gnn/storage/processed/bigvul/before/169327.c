void SimulateGestureFlingSequence(WebContents* web_contents,
                                  const gfx::Point& point,
                                  const gfx::Vector2dF& velocity) {
  RenderWidgetHostImpl* widget_host = RenderWidgetHostImpl::From(
      web_contents->GetRenderViewHost()->GetWidget());

  blink::WebGestureEvent scroll_begin(
      blink::WebGestureEvent::kGestureScrollBegin,
      blink::WebInputEvent::kNoModifiers, ui::EventTimeForNow(),
      blink::kWebGestureDeviceTouchpad);
  scroll_begin.SetPositionInWidget(gfx::PointF(point));
  widget_host->ForwardGestureEvent(scroll_begin);

  blink::WebGestureEvent scroll_end(blink::WebGestureEvent::kGestureScrollEnd,
                                    blink::WebInputEvent::kNoModifiers,
                                    ui::EventTimeForNow(),
                                    blink::kWebGestureDeviceTouchpad);
  scroll_end.SetPositionInWidget(gfx::PointF(point));
  widget_host->ForwardGestureEvent(scroll_end);

  blink::WebGestureEvent fling_start(blink::WebGestureEvent::kGestureFlingStart,
                                     blink::WebInputEvent::kNoModifiers,
                                     ui::EventTimeForNow(),
                                     blink::kWebGestureDeviceTouchpad);
  fling_start.SetPositionInWidget(gfx::PointF(point));
  fling_start.data.fling_start.target_viewport = false;
  fling_start.data.fling_start.velocity_x = velocity.x();
  fling_start.data.fling_start.velocity_y = velocity.y();
  widget_host->ForwardGestureEvent(fling_start);
}
