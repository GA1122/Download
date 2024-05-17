void HTMLMediaElement::noneSupported() {
  BLINK_MEDIA_LOG << "noneSupported(" << (void*)this << ")";

  stopPeriodicTimers();
  m_loadState = WaitingForSource;
  m_currentSourceNode = nullptr;


  m_error = MediaError::create(MediaError::kMediaErrSrcNotSupported);

  forgetResourceSpecificTracks();

  setNetworkState(kNetworkNoSource);

  updateDisplayState();

  scheduleEvent(EventTypeNames::error);

  scheduleRejectPlayPromises(NotSupportedError);

  closeMediaSource();

  setShouldDelayLoadEvent(false);

  if (layoutObject())
    layoutObject()->updateFromElement();
}
