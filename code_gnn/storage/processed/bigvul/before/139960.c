double HTMLMediaElement::currentTime() const {
  if (m_defaultPlaybackStartPosition)
    return m_defaultPlaybackStartPosition;

  if (m_seeking) {
    BLINK_MEDIA_LOG << "currentTime(" << (void*)this
                    << ") - seeking, returning " << m_lastSeekTime;
    return m_lastSeekTime;
  }

  return officialPlaybackPosition();
}
