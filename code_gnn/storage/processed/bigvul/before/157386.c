void HTMLMediaElement::ProgressEventTimerFired(TimerBase*) {
  if (network_state_ != kNetworkLoading)
    return;

  if (MediaShouldBeOpaque())
    return;

  double time = WTF::CurrentTime();
  double timedelta = time - previous_progress_time_;

  if (GetWebMediaPlayer() && GetWebMediaPlayer()->DidLoadingProgress()) {
    ScheduleEvent(EventTypeNames::progress);
    previous_progress_time_ = time;
    sent_stalled_event_ = false;
    if (GetLayoutObject())
      GetLayoutObject()->UpdateFromElement();
  } else if (!media_source_ && timedelta > 3.0 && !sent_stalled_event_) {
    ScheduleEvent(EventTypeNames::stalled);
    sent_stalled_event_ = true;
    SetShouldDelayLoadEvent(false);
  }
}
