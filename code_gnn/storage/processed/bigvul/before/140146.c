void HTMLMediaElement::waitForSourceChange() {
  BLINK_MEDIA_LOG << "waitForSourceChange(" << (void*)this << ")";

  stopPeriodicTimers();
  m_loadState = WaitingForSource;

  setNetworkState(kNetworkNoSource);

  setShouldDelayLoadEvent(false);

  updateDisplayState();

  if (layoutObject())
    layoutObject()->updateFromElement();
}
