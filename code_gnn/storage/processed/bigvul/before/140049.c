void HTMLMediaElement::playbackProgressTimerFired(TimerBase*) {
  if (!std::isnan(m_fragmentEndTime) && currentTime() >= m_fragmentEndTime &&
      getDirectionOfPlayback() == Forward) {
    m_fragmentEndTime = std::numeric_limits<double>::quiet_NaN();
    if (!m_paused) {
      UseCounter::count(document(),
                        UseCounter::HTMLMediaElementPauseAtFragmentEnd);
      pauseInternal();
    }
  }

  if (!m_seeking)
    scheduleTimeupdateEvent(true);

  if (!playbackRate())
    return;

  cueTimeline().updateActiveCues(currentTime());
}
