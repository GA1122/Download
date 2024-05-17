void HTMLMediaElement::scheduleTextTrackResourceLoad() {
  BLINK_MEDIA_LOG << "scheduleTextTrackResourceLoad(" << (void*)this << ")";

  m_pendingActionFlags |= LoadTextTrackResource;

  if (!m_loadTimer.isActive())
    m_loadTimer.startOneShot(0, BLINK_FROM_HERE);
}
