InputHandlerProxy::EventDisposition InputHandlerProxy::HandleMouseWheel(
    const WebMouseWheelEvent& wheel_event) {
  InputHandlerProxy::EventDisposition result = DROP_EVENT;

  if (wheel_event.dispatch_type == WebInputEvent::kEventNonBlocking) {
    DCHECK(wheel_event.phase != WebMouseWheelEvent::kPhaseBegan);
    DCHECK(wheel_event.phase != WebMouseWheelEvent::kPhaseNone ||
           wheel_event.momentum_phase != WebMouseWheelEvent::kPhaseNone);

    result = static_cast<EventDisposition>(mouse_wheel_result_);

    if (wheel_event.phase == WebMouseWheelEvent::kPhaseEnded ||
        wheel_event.phase == WebMouseWheelEvent::kPhaseCancelled ||
        wheel_event.momentum_phase == WebMouseWheelEvent::kPhaseEnded ||
        wheel_event.momentum_phase == WebMouseWheelEvent::kPhaseCancelled) {
      mouse_wheel_result_ = kEventDispositionUndefined;
    }
    if (mouse_wheel_result_ != kEventDispositionUndefined)
      return result;
  }

  blink::WebFloatPoint position_in_widget = wheel_event.PositionInWidget();
  if (input_handler_->HasBlockingWheelEventHandlerAt(
          gfx::Point(position_in_widget.x, position_in_widget.y))) {
    result = DID_NOT_HANDLE;
  } else {
    cc::EventListenerProperties properties =
        input_handler_->GetEventListenerProperties(
            cc::EventListenerClass::kMouseWheel);
    switch (properties) {
      case cc::EventListenerProperties::kBlockingAndPassive:
      case cc::EventListenerProperties::kPassive:
        result = DID_HANDLE_NON_BLOCKING;
        break;
      case cc::EventListenerProperties::kNone:
        result = DROP_EVENT;
        break;
      default:
        result = DROP_EVENT;
    }
  }

  mouse_wheel_result_ = result;
  return result;
}
