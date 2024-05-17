void HTMLMediaElement::mediaEngineError(MediaError* err) {
  DCHECK_GE(m_readyState, kHaveMetadata);
  BLINK_MEDIA_LOG << "mediaEngineError(" << (void*)this << ", "
                  << static_cast<int>(err->code()) << ")";

  stopPeriodicTimers();
  m_loadState = WaitingForSource;

  m_error = err;

  scheduleEvent(EventTypeNames::error);

  setNetworkState(kNetworkIdle);

  setShouldDelayLoadEvent(false);

  m_currentSourceNode = nullptr;
}
