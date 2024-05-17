void HTMLMediaElement::setOfficialPlaybackPosition(double position) const {
#if LOG_OFFICIAL_TIME_STATUS
  BLINK_MEDIA_LOG << "setOfficialPlaybackPosition(" << (void*)this
                  << ") was:" << m_officialPlaybackPosition
                  << " now:" << position;
#endif

  m_officialPlaybackPosition =
      std::isnan(duration()) ? position : std::min(duration(), position);

  if (m_officialPlaybackPosition != position) {
    BLINK_MEDIA_LOG << "setOfficialPlaybackPosition(" << (void*)this
                    << ") position:" << position
                    << " truncated to duration:" << m_officialPlaybackPosition;
  }

  m_officialPlaybackPositionNeedsUpdate = false;
  Microtask::enqueueMicrotask(
      WTF::bind(&HTMLMediaElement::requireOfficialPlaybackPositionUpdate,
                wrapWeakPersistent(this)));
}
