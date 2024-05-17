Response InputHandler::EmulateTouchFromMouseEvent(const std::string& type,
                                                  int x,
                                                  int y,
                                                  const std::string& button,
                                                  Maybe<double> maybe_timestamp,
                                                  Maybe<double> delta_x,
                                                  Maybe<double> delta_y,
                                                  Maybe<int> modifiers,
                                                  Maybe<int> click_count) {
  blink::WebInputEvent::Type event_type;
  if (type == Input::EmulateTouchFromMouseEvent::TypeEnum::MouseWheel) {
    event_type = blink::WebInputEvent::kMouseWheel;
    if (!delta_x.isJust() || !delta_y.isJust()) {
      return Response::InvalidParams(
          "'deltaX' and 'deltaY' are expected for mouseWheel event");
    }
  } else {
    event_type = GetMouseEventType(type);
    if (event_type == blink::WebInputEvent::kUndefined) {
      return Response::InvalidParams(
          base::StringPrintf("Unexpected event type '%s'", type.c_str()));
    }
  }

  blink::WebPointerProperties::Button event_button =
      blink::WebPointerProperties::Button::kNoButton;
  int button_modifiers = 0;
  if (!GetMouseEventButton(button, &event_button, &button_modifiers))
    return Response::InvalidParams("Invalid mouse button");

  ui::WebScopedInputEvent event;
  blink::WebMouseWheelEvent* wheel_event = nullptr;
  blink::WebMouseEvent* mouse_event = nullptr;
  if (type == Input::EmulateTouchFromMouseEvent::TypeEnum::MouseWheel) {
    wheel_event = new blink::WebMouseWheelEvent(
        event_type,
        GetEventModifiers(
            modifiers.fromMaybe(blink::WebInputEvent::kNoModifiers), false,
            false, 0) |
            button_modifiers,
        GetEventTimestamp(maybe_timestamp));
    mouse_event = wheel_event;
    event.reset(wheel_event);
    wheel_event->delta_x = static_cast<float>(delta_x.fromJust());
    wheel_event->delta_y = static_cast<float>(delta_y.fromJust());
    if (base::FeatureList::IsEnabled(
            features::kTouchpadAndWheelScrollLatching)) {
      wheel_event->phase = blink::WebMouseWheelEvent::kPhaseBegan;
    }
  } else {
    mouse_event = new blink::WebMouseEvent(
        event_type,
        GetEventModifiers(
            modifiers.fromMaybe(blink::WebInputEvent::kNoModifiers), false,
            false, 0) |
            button_modifiers,
        GetEventTimestamp(maybe_timestamp));
    event.reset(mouse_event);
  }

  mouse_event->SetPositionInWidget(x, y);
  mouse_event->button = event_button;
  mouse_event->SetPositionInScreen(x, y);
  mouse_event->click_count = click_count.fromMaybe(0);
  mouse_event->pointer_type = blink::WebPointerProperties::PointerType::kTouch;

  if (!host_ || !host_->GetRenderWidgetHost())
    return Response::InternalError();

  if (wheel_event) {
    host_->GetRenderWidgetHost()->ForwardWheelEvent(*wheel_event);
    if (base::FeatureList::IsEnabled(
            features::kTouchpadAndWheelScrollLatching)) {
      wheel_event->delta_x = 0;
      wheel_event->delta_y = 0;
      wheel_event->phase = blink::WebMouseWheelEvent::kPhaseEnded;
      wheel_event->dispatch_type = blink::WebInputEvent::kEventNonBlocking;
      host_->GetRenderWidgetHost()->ForwardWheelEvent(*wheel_event);
    }
  } else {
    host_->GetRenderWidgetHost()->ForwardMouseEvent(*mouse_event);
  }
  return Response::OK();
}
