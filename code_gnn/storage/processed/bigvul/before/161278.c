bool GetMouseEventButton(const std::string& button,
                         blink::WebPointerProperties::Button* event_button,
                         int* event_modifiers) {
  if (button.empty())
    return true;

  if (button == Input::DispatchMouseEvent::ButtonEnum::None) {
    *event_button = blink::WebMouseEvent::Button::kNoButton;
  } else if (button == Input::DispatchMouseEvent::ButtonEnum::Left) {
    *event_button = blink::WebMouseEvent::Button::kLeft;
    *event_modifiers = blink::WebInputEvent::kLeftButtonDown;
  } else if (button == Input::DispatchMouseEvent::ButtonEnum::Middle) {
    *event_button = blink::WebMouseEvent::Button::kMiddle;
    *event_modifiers = blink::WebInputEvent::kMiddleButtonDown;
  } else if (button == Input::DispatchMouseEvent::ButtonEnum::Right) {
    *event_button = blink::WebMouseEvent::Button::kRight;
    *event_modifiers = blink::WebInputEvent::kRightButtonDown;
  } else {
    return false;
  }
  return true;
}
