TimeRanges* HTMLMediaElement::buffered() const {
  if (m_mediaSource)
    return m_mediaSource->buffered();

  if (!webMediaPlayer())
    return TimeRanges::create();

  return TimeRanges::create(webMediaPlayer()->buffered());
}
