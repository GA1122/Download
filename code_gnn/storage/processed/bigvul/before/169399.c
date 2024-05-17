InputMsgWatcher::~InputMsgWatcher() {
  render_widget_host_->RemoveInputEventObserver(this);
}
