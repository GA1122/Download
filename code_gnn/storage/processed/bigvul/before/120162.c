void Layer::SetHaveWheelEventHandlers(bool have_wheel_event_handlers) {
  DCHECK(IsPropertyChangeAllowed());
  if (have_wheel_event_handlers_ == have_wheel_event_handlers)
    return;
  have_wheel_event_handlers_ = have_wheel_event_handlers;
  SetNeedsCommit();
}
