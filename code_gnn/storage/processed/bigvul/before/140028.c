WebMediaPlayer::LoadType HTMLMediaElement::loadType() const {
  if (m_mediaSource)
    return WebMediaPlayer::LoadTypeMediaSource;

  if (m_srcObject ||
      (!m_currentSrc.isNull() && isMediaStreamURL(m_currentSrc.getString())))
    return WebMediaPlayer::LoadTypeMediaStream;

  return WebMediaPlayer::LoadTypeURL;
}
