void RenderProcessHostImpl::SetIgnoreInputEvents(bool ignore_input_events) {
  if (ignore_input_events == ignore_input_events_)
    return;

  ignore_input_events_ = ignore_input_events;
  for (auto* widget : widgets_) {
    widget->ProcessIgnoreInputEventsChanged(ignore_input_events);
  }
}
