void HTMLMediaElement::setDefaultPlaybackRate(double rate) {
  if (default_playback_rate_ == rate)
    return;

  default_playback_rate_ = rate;
  ScheduleEvent(EventTypeNames::ratechange);
}
