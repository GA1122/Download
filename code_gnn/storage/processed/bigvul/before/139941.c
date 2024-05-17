void HTMLMediaElement::cancelDeferredLoad() {
  m_deferredLoadTimer.stop();
  m_deferredLoadState = NotDeferred;
}
