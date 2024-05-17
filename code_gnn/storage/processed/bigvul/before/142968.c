void HTMLMediaElement::setDefaultPlaybackRate(double rate) {
  if (GetLoadType() == WebMediaPlayer::kLoadTypeMediaStream)
    return;

  if (default_playback_rate_ == rate)
    return;

  default_playback_rate_ = rate;
  ScheduleEvent(event_type_names::kRatechange);
}
