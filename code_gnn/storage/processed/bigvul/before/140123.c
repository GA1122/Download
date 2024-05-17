void HTMLMediaElement::startProgressEventTimer() {
  if (m_progressEventTimer.isActive())
    return;

  m_previousProgressTime = WTF::currentTime();
  m_progressEventTimer.startRepeating(0.350, BLINK_FROM_HERE);
}
