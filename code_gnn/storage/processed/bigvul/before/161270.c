void InputHandler::DispatchMouseEvent(
    const std::string& event_type,
    double x,
    double y,
    Maybe<int> maybe_modifiers,
    Maybe<double> maybe_timestamp,
    Maybe<std::string> maybe_button,
    Maybe<int> click_count,
    Maybe<double> delta_x,
    Maybe<double> delta_y,
    std::unique_ptr<DispatchMouseEventCallback> callback) {
  blink::WebInputEvent::Type type = GetMouseEventType(event_type);
  if (type == blink::WebInputEvent::kUndefined) {
    callback->sendFailure(Response::InvalidParams(
        base::StringPrintf("Unexpected event type '%s'", event_type.c_str())));
    return;
  }

  blink::WebPointerProperties::Button button =
      blink::WebPointerProperties::Button::kNoButton;
  int button_modifiers = 0;
  if (!GetMouseEventButton(maybe_button.fromMaybe(""), &button,
                           &button_modifiers)) {
    callback->sendFailure(Response::InvalidParams("Invalid mouse button"));
    return;
  }

  int modifiers = GetEventModifiers(
      maybe_modifiers.fromMaybe(blink::WebInputEvent::kNoModifiers), false,
      false, 0);
  modifiers |= button_modifiers;
  double timestamp = GetEventTimestamp(maybe_timestamp);

  std::unique_ptr<blink::WebMouseEvent, ui::WebInputEventDeleter> mouse_event;
  blink::WebMouseWheelEvent* wheel_event = nullptr;

  if (type == blink::WebInputEvent::kMouseWheel) {
    wheel_event = new blink::WebMouseWheelEvent(type, modifiers, timestamp);
    mouse_event.reset(wheel_event);
    if (!delta_x.isJust() || !delta_y.isJust()) {
      callback->sendFailure(Response::InvalidParams(
          "'deltaX' and 'deltaY' are expected for mouseWheel event"));
      return;
    }
    wheel_event->delta_x = static_cast<float>(-delta_x.fromJust());
    wheel_event->delta_y = static_cast<float>(-delta_y.fromJust());
    if (base::FeatureList::IsEnabled(
            features::kTouchpadAndWheelScrollLatching)) {
      wheel_event->phase = blink::WebMouseWheelEvent::kPhaseBegan;
      wheel_event->dispatch_type = blink::WebInputEvent::kBlocking;
    }
  } else {
    mouse_event.reset(new blink::WebMouseEvent(type, modifiers, timestamp));
  }

  mouse_event->button = button;
  mouse_event->SetPositionInWidget(x * page_scale_factor_,
                                   y * page_scale_factor_);
  mouse_event->SetPositionInScreen(x * page_scale_factor_,
                                   y * page_scale_factor_);
  mouse_event->click_count = click_count.fromMaybe(0);
  mouse_event->pointer_type = blink::WebPointerProperties::PointerType::kMouse;

  if (!host_ || !host_->GetRenderWidgetHost()) {
    callback->sendFailure(Response::InternalError());
    return;
  }

  host_->GetRenderWidgetHost()->Focus();
  input_queued_ = false;
  pending_mouse_callbacks_.push_back(std::move(callback));
  if (wheel_event)
    host_->GetRenderWidgetHost()->ForwardWheelEvent(*wheel_event);
  else
    host_->GetRenderWidgetHost()->ForwardMouseEvent(*mouse_event);
  if (!input_queued_) {
    pending_mouse_callbacks_.back()->sendSuccess();
    pending_mouse_callbacks_.pop_back();
  } else if (wheel_event && base::FeatureList::IsEnabled(
                                features::kTouchpadAndWheelScrollLatching)) {
    wheel_event->delta_x = 0;
    wheel_event->delta_y = 0;
    wheel_event->phase = blink::WebMouseWheelEvent::kPhaseEnded;
    wheel_event->dispatch_type = blink::WebInputEvent::kEventNonBlocking;
    host_->GetRenderWidgetHost()->ForwardWheelEvent(*wheel_event);
  }
}
