Response InputHandler::Disable() {
  ClearInputState();
  if (host_) {
    host_->GetRenderWidgetHost()->RemoveInputEventObserver(this);
    if (ignore_input_events_)
      host_->GetRenderWidgetHost()->SetIgnoreInputEvents(false);
  }
  ignore_input_events_ = false;
  touch_points_.clear();
  return Response::OK();
}
