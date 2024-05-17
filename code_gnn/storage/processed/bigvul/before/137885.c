void MediaControlPanelElement::startTimer() {
  stopTimer();

  m_transitionTimer.startOneShot(fadeOutDuration, BLINK_FROM_HERE);
}
