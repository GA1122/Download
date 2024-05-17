double HTMLMediaElement::currentPlaybackPosition() const {
  if (m_readyState == kHaveNothing)
    return 0;

  if (webMediaPlayer())
    return webMediaPlayer()->currentTime();

  if (m_readyState >= kHaveMetadata) {
    LOG(WARNING) << __func__ << " readyState = " << m_readyState
                 << " but no webMeidaPlayer to provide currentPlaybackPosition";
  }

  return 0;
}
