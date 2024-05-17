void RenderWidgetHostImpl::ProcessIgnoreInputEventsChanged(
    bool ignore_input_events) {
  if (ignore_input_events)
    StopInputEventAckTimeout();
  else
    RestartInputEventAckTimeoutIfNecessary();
}
