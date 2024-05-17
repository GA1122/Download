bool IsResourceLoadUserInteractionInputType(blink::WebInputEvent::Type type) {
  return type == blink::WebInputEvent::kMouseDown ||
         type == blink::WebInputEvent::kTouchStart ||
         type == blink::WebInputEvent::kRawKeyDown;
}
