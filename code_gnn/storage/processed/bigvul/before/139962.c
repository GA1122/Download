void HTMLMediaElement::deferLoad() {
  DCHECK(!m_deferredLoadTimer.isActive());
  DCHECK_EQ(m_deferredLoadState, NotDeferred);
  changeNetworkStateFromLoadingToIdle();
  m_deferredLoadTimer.startOneShot(0, BLINK_FROM_HERE);
  m_deferredLoadState = WaitingForStopDelayingLoadEventTask;
}
