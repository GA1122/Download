void TouchEventConverterEvdev::ReportEvents(base::TimeDelta delta) {
  if (touch_noise_finder_)
    touch_noise_finder_->HandleTouches(events_, delta);

  for (size_t i = 0; i < events_.size(); i++) {
    InProgressTouchEvdev* event = &events_[i];
    if (!event->altered)
      continue;

    EventType event_type = GetEventTypeForTouch(*event);
    if (event_type == ET_UNKNOWN || event_type == ET_TOUCH_CANCELLED)
      event->cancelled = true;

    if (event_type != ET_UNKNOWN)
      ReportEvent(*event, event_type, delta);

    event->was_touching = event->touching;
    event->altered = false;
  }
}
