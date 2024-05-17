double HTMLMediaElement::officialPlaybackPosition() const {
  bool waitingForData = m_readyState <= kHaveCurrentData;
  if (m_officialPlaybackPositionNeedsUpdate && !m_paused && !waitingForData) {
    setOfficialPlaybackPosition(currentPlaybackPosition());
  }

#if LOG_OFFICIAL_TIME_STATUS
  static const double minCachedDeltaForWarning = 0.01;
  double delta =
      std::abs(m_officialPlaybackPosition - currentPlaybackPosition());
  if (delta > minCachedDeltaForWarning) {
    BLINK_MEDIA_LOG << "currentTime(" << (void*)this
                    << ") - WARNING, cached time is " << delta
                    << "seconds off of media time when paused/waiting";
  }
#endif

  return m_officialPlaybackPosition;
}
