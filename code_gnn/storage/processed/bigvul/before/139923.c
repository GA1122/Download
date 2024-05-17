void HTMLMediaElement::activateViewportIntersectionMonitoring(bool activate) {
  if (activate && !m_checkViewportIntersectionTimer.isActive()) {
    m_checkViewportIntersectionTimer.startRepeating(
        kCheckViewportIntersectionIntervalSeconds, BLINK_FROM_HERE);
  } else if (!activate) {
    m_checkViewportIntersectionTimer.stop();
  }
}
