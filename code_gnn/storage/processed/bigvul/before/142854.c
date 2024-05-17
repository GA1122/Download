void HTMLMediaElement::PlaybackProgressTimerFired(TimerBase*) {
  if (!std::isnan(fragment_end_time_) && currentTime() >= fragment_end_time_ &&
      GetDirectionOfPlayback() == kForward) {
    fragment_end_time_ = std::numeric_limits<double>::quiet_NaN();
    if (!paused_) {
      UseCounter::Count(GetDocument(),
                        WebFeature::kHTMLMediaElementPauseAtFragmentEnd);
      PauseInternal();
    }
  }

  if (!seeking_)
    ScheduleTimeupdateEvent(true);

  if (!playbackRate())
    return;

  GetCueTimeline().UpdateActiveCues(currentTime());
}
