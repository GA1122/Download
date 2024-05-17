void HTMLMediaElement::scheduleTimeupdateEvent(bool periodicEvent) {
  double mediaTime = currentPlaybackPosition();
  double now = WTF::currentTime();

  bool haveNotRecentlyFiredTimeupdate =
      (now - m_lastTimeUpdateEventWallTime) >= maxTimeupdateEventFrequency;
  bool mediaTimeHasProgressed = mediaTime != m_lastTimeUpdateEventMediaTime;

  if (!periodicEvent ||
      (haveNotRecentlyFiredTimeupdate && mediaTimeHasProgressed)) {
    scheduleEvent(EventTypeNames::timeupdate);
    m_lastTimeUpdateEventWallTime = now;
    m_lastTimeUpdateEventMediaTime = mediaTime;
  }
}
