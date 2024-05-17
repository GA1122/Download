void RenderWidgetHostImpl::ProcessWheelAck(bool processed) {
  mouse_wheel_pending_ = false;

  if (overscroll_controller_.get())
    overscroll_controller_->ReceivedEventACK(current_wheel_event_, processed);

  if (!coalesced_mouse_wheel_events_.empty()) {
    WebMouseWheelEvent next_wheel_event =
        coalesced_mouse_wheel_events_.front();
    coalesced_mouse_wheel_events_.pop_front();
    ForwardWheelEvent(next_wheel_event);
  }

  if (!processed && !is_hidden_ && view_)
    view_->UnhandledWheelEvent(current_wheel_event_);
}
