void HTMLMediaElement::load() {
  BLINK_MEDIA_LOG << "load(" << (void*)this << ")";

  if (isLockedPendingUserGesture() &&
      UserGestureIndicator::utilizeUserGesture()) {
    unlockUserGesture();
  }

  m_ignorePreloadNone = true;
  invokeLoadAlgorithm();
}
