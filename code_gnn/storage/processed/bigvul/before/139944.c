void HTMLMediaElement::changeNetworkStateFromLoadingToIdle() {
  m_progressEventTimer.stop();

  if (webMediaPlayer() && webMediaPlayer()->didLoadingProgress())
    scheduleEvent(EventTypeNames::progress);
  scheduleEvent(EventTypeNames::suspend);
  setNetworkState(kNetworkIdle);
}
