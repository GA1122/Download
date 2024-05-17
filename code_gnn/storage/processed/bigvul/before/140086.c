TimeRanges* HTMLMediaElement::seekable() const {
  if (!webMediaPlayer())
    return TimeRanges::create();

  if (m_mediaSource)
    return m_mediaSource->seekable();

  return TimeRanges::create(webMediaPlayer()->seekable());
}
