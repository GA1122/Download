void HTMLMediaElement::setPlaybackRate(double rate) {
  BLINK_MEDIA_LOG << "setPlaybackRate(" << (void*)this << ", " << rate << ")";

  if (m_playbackRate != rate) {
    m_playbackRate = rate;
    scheduleEvent(EventTypeNames::ratechange);
  }

  updatePlaybackRate();
}
