void RenderWidgetHostImpl::OnMsgInputEventAck(
    WebInputEvent::Type event_type, InputEventAckState ack_result) {
  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::OnMsgInputEventAck");
  bool processed = (ack_result == INPUT_EVENT_ACK_STATE_CONSUMED);

  if (!in_process_event_types_.empty() &&
      in_process_event_types_.front() == event_type)
    in_process_event_types_.pop();

  TimeDelta delta = TimeTicks::Now() - input_event_start_time_;
  UMA_HISTOGRAM_TIMES("MPArch.RWH_InputEventDelta", delta);

  if (decrement_in_flight_event_count() == 0)
    StopHangMonitorTimeout();

  if (in_process_event_types_.empty() &&
      !active_smooth_scroll_gestures_.empty())
    TickActiveSmoothScrollGesture();

  int type = static_cast<int>(event_type);
  if (type < WebInputEvent::Undefined) {
    RecordAction(UserMetricsAction("BadMessageTerminate_RWH2"));
    process_->ReceivedBadMessage();
  } else if (type == WebInputEvent::MouseMove) {
    mouse_move_pending_ = false;

    if (next_mouse_move_.get()) {
      DCHECK(next_mouse_move_->type == WebInputEvent::MouseMove);
      ForwardMouseEvent(*next_mouse_move_);
    }
  } else if (WebInputEvent::isKeyboardEventType(type)) {
    ProcessKeyboardEventAck(type, processed);
  } else if (type == WebInputEvent::MouseWheel) {
    ProcessWheelAck(processed);
  } else if (WebInputEvent::isTouchEventType(type)) {
    ProcessTouchAck(ack_result);
  } else if (WebInputEvent::isGestureEventType(type)) {
    ProcessGestureAck(processed, type);
  }


  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DID_RECEIVE_INPUT_EVENT_ACK,
      Source<void>(this),
      Details<int>(&type));
}
