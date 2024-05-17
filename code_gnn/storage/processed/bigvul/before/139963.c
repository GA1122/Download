void HTMLMediaElement::deferredLoadTimerFired(TimerBase*) {
  setShouldDelayLoadEvent(false);

  if (m_deferredLoadState == ExecuteOnStopDelayingLoadEventTask) {
    executeDeferredLoad();
    return;
  }
  DCHECK_EQ(m_deferredLoadState, WaitingForStopDelayingLoadEventTask);
  m_deferredLoadState = WaitingForTrigger;
}
