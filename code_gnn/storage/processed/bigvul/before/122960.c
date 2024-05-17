void RenderWidgetHostImpl::ForwardInputEvent(const WebInputEvent& input_event,
                                             int event_size,
                                             bool is_keyboard_shortcut) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::ForwardInputEvent");

  if (!process_->HasConnection())
    return;

  DCHECK(!process_->IgnoreInputEvents());

  if (overscroll_controller_.get() &&
      !overscroll_controller_->WillDispatchEvent(input_event)) {
    if (input_event.type == WebKit::WebInputEvent::MouseWheel) {
      mouse_wheel_pending_ = false;
    } else if (WebInputEvent::isGestureEventType(input_event.type) &&
               gesture_event_filter_->HasQueuedGestureEvents()) {
      gesture_event_filter_->ProcessGestureAck(true, input_event.type);
    } else if (WebInputEvent::isTouchEventType(input_event.type)) {
      touch_event_queue_->ProcessTouchAck(INPUT_EVENT_ACK_STATE_NOT_CONSUMED);
    }
    return;
  }

  in_process_event_types_.push(input_event.type);

  if (input_event.type != WebInputEvent::MouseWheel) {
    for (size_t i = 0; i < coalesced_mouse_wheel_events_.size(); ++i) {
      SendInputEvent(coalesced_mouse_wheel_events_[i],
                     sizeof(WebMouseWheelEvent), false);
    }
    coalesced_mouse_wheel_events_.clear();
  }

  SendInputEvent(input_event, event_size, is_keyboard_shortcut);

  next_mouse_move_.reset();

  StartHangMonitorTimeout(
      TimeDelta::FromMilliseconds(hung_renderer_delay_ms_));
}
