void HTMLMediaElement::setDefaultPlaybackRate(double rate) {
  if (m_defaultPlaybackRate == rate)
    return;

  m_defaultPlaybackRate = rate;
  scheduleEvent(EventTypeNames::ratechange);
}
