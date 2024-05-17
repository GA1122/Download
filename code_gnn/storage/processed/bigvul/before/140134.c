void HTMLMediaElement::timeChanged() {
  BLINK_MEDIA_LOG << "timeChanged(" << (void*)this << ")";

  cueTimeline().updateActiveCues(currentTime());

  if (m_seeking && m_readyState >= kHaveCurrentData &&
      !webMediaPlayer()->seeking())
    finishSeek();

  scheduleTimeupdateEvent(false);

  double now = currentPlaybackPosition();
  double dur = duration();

  if (!std::isnan(dur) && dur && now >= dur &&
      getDirectionOfPlayback() == Forward) {
    if (loop()) {
      seek(earliestPossiblePosition());
    } else {
      if (!m_paused) {
        m_paused = true;
        scheduleEvent(EventTypeNames::pause);
        scheduleRejectPlayPromises(AbortError);
      }
      scheduleEvent(EventTypeNames::ended);
    }
  }
  updatePlayState();
}
