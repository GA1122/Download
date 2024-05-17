void HTMLMediaElement::contextDestroyed(ExecutionContext*) {
  BLINK_MEDIA_LOG << "contextDestroyed(" << (void*)this << ")";

  cancelPendingEventsAndCallbacks();
  m_asyncEventQueue->close();

  clearMediaPlayer();
  m_readyState = kHaveNothing;
  m_readyStateMaximum = kHaveNothing;
  setNetworkState(kNetworkEmpty);
  setShouldDelayLoadEvent(false);
  m_currentSourceNode = nullptr;
  m_officialPlaybackPosition = 0;
  m_officialPlaybackPositionNeedsUpdate = true;
  cueTimeline().updateActiveCues(0);
  m_playing = false;
  m_paused = true;
  m_seeking = false;

  if (layoutObject())
    layoutObject()->updateFromElement();

  stopPeriodicTimers();

  DCHECK(!hasPendingActivity());
}
