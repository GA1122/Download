void HTMLMediaElement::unlockUserGesture() {
  m_lockedPendingUserGesture = false;
  m_lockedPendingUserGestureIfCrossOriginExperimentEnabled = false;
}
