void HTMLMediaElement::startPlaybackProgressTimer() {
  if (m_playbackProgressTimer.isActive())
    return;

  m_previousProgressTime = WTF::currentTime();
  m_playbackProgressTimer.startRepeating(maxTimeupdateEventFrequency,
                                         BLINK_FROM_HERE);
}
