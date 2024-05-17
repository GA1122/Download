TimeRanges* HTMLMediaElement::played() {
  if (m_playing) {
    double time = currentTime();
    if (time > m_lastSeekTime)
      addPlayedRange(m_lastSeekTime, time);
  }

  if (!m_playedTimeRanges)
    m_playedTimeRanges = TimeRanges::create();

  return m_playedTimeRanges->copy();
}
