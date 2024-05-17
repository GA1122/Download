void HTMLMediaElement::executeDeferredLoad() {
  DCHECK_GE(m_deferredLoadState, WaitingForTrigger);


  cancelDeferredLoad();
  setShouldDelayLoadEvent(true);
  setNetworkState(kNetworkLoading);

  startProgressEventTimer();

  startPlayerLoad();
}
