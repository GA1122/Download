void HTMLMediaElement::startDeferredLoad() {
  if (m_deferredLoadState == WaitingForTrigger) {
    executeDeferredLoad();
    return;
  }
  if (m_deferredLoadState == ExecuteOnStopDelayingLoadEventTask)
    return;
  DCHECK_EQ(m_deferredLoadState, WaitingForStopDelayingLoadEventTask);
  m_deferredLoadState = ExecuteOnStopDelayingLoadEventTask;
}
