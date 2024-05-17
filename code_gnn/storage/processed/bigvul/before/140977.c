void Document::CheckLoadEventSoon() {
  if (GetFrame() && !load_event_delay_timer_.IsActive())
    load_event_delay_timer_.StartOneShot(TimeDelta(), FROM_HERE);
}
