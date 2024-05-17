void HTMLMediaElement::playInternal() {
  BLINK_MEDIA_LOG << "playInternal(" << (void*)this << ")";

  if (webMediaPlayer())
    webMediaPlayer()->setBufferingStrategy(
        WebMediaPlayer::BufferingStrategy::Normal);

  if (m_networkState == kNetworkEmpty)
    invokeResourceSelectionAlgorithm();

  if (endedPlayback(LoopCondition::Ignored))
    seek(0);

  if (m_paused) {
    m_paused = false;
    scheduleEvent(EventTypeNames::play);

    if (m_readyState <= kHaveCurrentData)
      scheduleEvent(EventTypeNames::waiting);
    else if (m_readyState >= kHaveFutureData)
      scheduleNotifyPlaying();
  } else if (m_readyState >= kHaveFutureData) {
    scheduleResolvePlayPromises();
  }

  m_autoplaying = false;

  setIgnorePreloadNone();
  updatePlayState();
}
