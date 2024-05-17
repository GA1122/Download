void HTMLMediaElement::scheduleNextSourceChild() {
  m_pendingActionFlags |= LoadMediaResource;
  m_loadTimer.startOneShot(0, BLINK_FROM_HERE);
}
