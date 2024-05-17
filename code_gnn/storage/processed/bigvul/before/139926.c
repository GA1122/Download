void HTMLMediaElement::addPlayedRange(double start, double end) {
  BLINK_MEDIA_LOG << "addPlayedRange(" << (void*)this << ", " << start << ", "
                  << end << ")";
  if (!m_playedTimeRanges)
    m_playedTimeRanges = TimeRanges::create();
  m_playedTimeRanges->add(start, end);
}
