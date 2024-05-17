void HTMLMediaElement::stopPeriodicTimers() {
  m_progressEventTimer.stop();
  m_playbackProgressTimer.stop();
  m_checkViewportIntersectionTimer.stop();
}
