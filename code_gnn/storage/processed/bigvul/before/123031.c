void RenderWidgetHostImpl::ProcessTouchAck(InputEventAckState ack_result) {
  touch_event_queue_->ProcessTouchAck(ack_result);
}
