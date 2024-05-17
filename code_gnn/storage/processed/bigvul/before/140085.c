void HTMLMediaElement::seek(double time) {
  BLINK_MEDIA_LOG << "seek(" << (void*)this << ", " << time << ")";

  if (!m_webMediaPlayer || m_readyState == kHaveNothing)
    return;

  setIgnorePreloadNone();

  double now = currentTime();


  m_seeking = true;

  time = std::min(time, duration());

  time = std::max(time, earliestPossiblePosition());

  double mediaTime = webMediaPlayer()->mediaTimeForTimeValue(time);
  if (time != mediaTime) {
    BLINK_MEDIA_LOG << "seek(" << (void*)this << ", " << time
                    << ") - media timeline equivalent is " << mediaTime;
    time = mediaTime;
  }

  TimeRanges* seekableRanges = seekable();

  if (!seekableRanges->length()) {
    m_seeking = false;
    return;
  }
  time = seekableRanges->nearest(time, now);

  if (m_playing && m_lastSeekTime < now)
    addPlayedRange(m_lastSeekTime, now);

  m_lastSeekTime = time;

  scheduleEvent(EventTypeNames::seeking);

  webMediaPlayer()->seek(time);

}
