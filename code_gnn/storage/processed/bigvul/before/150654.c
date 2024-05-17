InputHandlerProxy::EventDisposition InputHandlerProxy::HandleTouchStart(
    const blink::WebTouchEvent& touch_event) {
  bool is_touching_scrolling_layer;
  cc::TouchAction white_listed_touch_action = cc::kTouchActionAuto;
  EventDisposition result = HitTestTouchEvent(
      touch_event, &is_touching_scrolling_layer, &white_listed_touch_action);

  if (result == DROP_EVENT &&
      input_handler_->GetEventListenerProperties(
          cc::EventListenerClass::kTouchEndOrCancel) !=
          cc::EventListenerProperties::kNone) {
    result = DID_HANDLE_NON_BLOCKING;
  }

  bool is_in_inertial_scrolling_on_impl =
      in_inertial_scrolling_ && gesture_scroll_on_impl_thread_;
  if (is_in_inertial_scrolling_on_impl && is_touching_scrolling_layer)
    result = DID_NOT_HANDLE_NON_BLOCKING_DUE_TO_FLING;

  client_->SetWhiteListedTouchAction(white_listed_touch_action,
                                     touch_event.unique_touch_event_id, result);

  return result;
}
