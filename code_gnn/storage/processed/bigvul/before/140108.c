void HTMLMediaElement::setShouldDelayLoadEvent(bool shouldDelay) {
  if (m_shouldDelayLoadEvent == shouldDelay)
    return;

  BLINK_MEDIA_LOG << "setShouldDelayLoadEvent(" << (void*)this << ", "
                  << boolString(shouldDelay) << ")";

  m_shouldDelayLoadEvent = shouldDelay;
  if (shouldDelay)
    document().incrementLoadEventDelayCount();
  else
    document().decrementLoadEventDelayCount();
}
