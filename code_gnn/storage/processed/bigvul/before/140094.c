void HTMLMediaElement::setCurrentTime(double time) {
  if (m_readyState == kHaveNothing) {
    m_defaultPlaybackStartPosition = time;
    return;
  }

  seek(time);
}
