InputHandlerProxy::EventDisposition InputHandlerProxy::HandleTouchMove(
    const blink::WebTouchEvent& touch_event) {
  if (touch_result_ == kEventDispositionUndefined ||
      touch_event.touch_start_or_first_touch_move) {
    bool is_touching_scrolling_layer;
    cc::TouchAction white_listed_touch_action = cc::kTouchActionAuto;
    EventDisposition result = HitTestTouchEvent(
        touch_event, &is_touching_scrolling_layer, &white_listed_touch_action);
    client_->SetWhiteListedTouchAction(
        white_listed_touch_action, touch_event.unique_touch_event_id, result);
    return result;
  }
  return static_cast<EventDisposition>(touch_result_);
}
