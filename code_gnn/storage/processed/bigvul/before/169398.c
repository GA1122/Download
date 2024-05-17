InputEventAckWaiter::~InputEventAckWaiter() {
  render_widget_host_->RemoveInputEventObserver(this);
}
