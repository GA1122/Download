void HTMLMediaElement::setIgnorePreloadNone() {
  BLINK_MEDIA_LOG << "setIgnorePreloadNone(" << (void*)this << ")";
  m_ignorePreloadNone = true;
  setPlayerPreload();
}
