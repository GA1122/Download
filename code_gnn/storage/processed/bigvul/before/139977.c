void HTMLMediaElement::durationChanged(double duration, bool requestSeek) {
  BLINK_MEDIA_LOG << "durationChanged(" << (void*)this << ", " << duration
                  << ", " << boolString(requestSeek) << ")";

  if (m_duration == duration)
    return;

  BLINK_MEDIA_LOG << "durationChanged(" << (void*)this << ") : " << m_duration
                  << " -> " << duration;
  m_duration = duration;
  scheduleEvent(EventTypeNames::durationchange);

  if (layoutObject())
    layoutObject()->updateFromElement();

  if (requestSeek)
    seek(duration);
}
