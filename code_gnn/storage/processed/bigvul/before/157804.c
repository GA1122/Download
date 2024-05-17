bool IsUserInteractionInputType(blink::WebInputEvent::Type type) {
  return type == blink::WebInputEvent::kMouseDown ||
         type == blink::WebInputEvent::kGestureScrollBegin ||
         type == blink::WebInputEvent::kTouchStart ||
         type == blink::WebInputEvent::kRawKeyDown;
}
