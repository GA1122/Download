void HTMLMediaElement::durationChanged() {
  BLINK_MEDIA_LOG << "durationChanged(" << (void*)this << ")";

  CHECK(m_webMediaPlayer);
  double newDuration = m_webMediaPlayer->duration();

  durationChanged(newDuration, currentPlaybackPosition() > newDuration);
}
