void HTMLMediaElement::finishSeek() {
  BLINK_MEDIA_LOG << "finishSeek(" << (void*)this << ")";

  m_seeking = false;

  setOfficialPlaybackPosition(currentPlaybackPosition());

  scheduleTimeupdateEvent(false);

  scheduleEvent(EventTypeNames::seeked);

  setDisplayMode(Video);
}
