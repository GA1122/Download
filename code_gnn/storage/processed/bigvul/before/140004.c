void HTMLMediaElement::invokeResourceSelectionAlgorithm() {
  BLINK_MEDIA_LOG << "invokeResourceSelectionAlgorithm(" << (void*)this << ")";
  setNetworkState(kNetworkNoSource);


  m_playedTimeRanges = TimeRanges::create();

  m_lastSeekTime = 0;
  m_duration = std::numeric_limits<double>::quiet_NaN();

  setShouldDelayLoadEvent(true);
  if (mediaControls())
    mediaControls()->reset();

  scheduleNextSourceChild();
}
