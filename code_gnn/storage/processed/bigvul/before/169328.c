void SimulateGesturePinchSequence(WebContents* web_contents,
                                  const gfx::Point& point,
                                  float scale,
                                  blink::WebGestureDevice source_device) {
  RenderWidgetHostImpl* widget_host = RenderWidgetHostImpl::From(
      web_contents->GetRenderViewHost()->GetWidget());

  blink::WebGestureEvent pinch_begin(blink::WebInputEvent::kGesturePinchBegin,
                                     blink::WebInputEvent::kNoModifiers,
                                     ui::EventTimeForNow(), source_device);
  pinch_begin.SetPositionInWidget(gfx::PointF(point));
  pinch_begin.SetPositionInScreen(gfx::PointF(point));
  widget_host->ForwardGestureEvent(pinch_begin);

  blink::WebGestureEvent pinch_update(pinch_begin);
  pinch_update.SetType(blink::WebInputEvent::kGesturePinchUpdate);
  pinch_update.data.pinch_update.scale = scale;
  widget_host->ForwardGestureEvent(pinch_update);

  blink::WebGestureEvent pinch_end(pinch_begin);
  pinch_update.SetType(blink::WebInputEvent::kGesturePinchEnd);
  widget_host->ForwardGestureEvent(pinch_end);
}
