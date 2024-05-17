blink::WebInputEvent::Type GetTouchEventType(const std::string& type) {
  if (type == Input::DispatchTouchEvent::TypeEnum::TouchStart)
    return blink::WebInputEvent::kTouchStart;
  if (type == Input::DispatchTouchEvent::TypeEnum::TouchEnd)
    return blink::WebInputEvent::kTouchEnd;
  if (type == Input::DispatchTouchEvent::TypeEnum::TouchMove)
    return blink::WebInputEvent::kTouchMove;
  if (type == Input::DispatchTouchEvent::TypeEnum::TouchCancel)
    return blink::WebInputEvent::kTouchCancel;
  return blink::WebInputEvent::kUndefined;
}
