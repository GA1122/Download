cc::InputHandler::ScrollInputType GestureScrollInputType(
    blink::WebGestureDevice device) {
  switch (device) {
    case blink::WebGestureDevice::kTouchpad:
      return cc::InputHandler::WHEEL;
    case blink::WebGestureDevice::kTouchscreen:
      return cc::InputHandler::TOUCHSCREEN;
    case blink::WebGestureDevice::kSyntheticAutoscroll:
      return cc::InputHandler::AUTOSCROLL;
    case blink::WebGestureDevice::kScrollbar:
      return cc::InputHandler::SCROLLBAR;
    case blink::WebGestureDevice::kUninitialized:
      break;
  }
  NOTREACHED();
  return cc::InputHandler::SCROLL_INPUT_UNKNOWN;
}
