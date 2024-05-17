void MediaControlPanelElement::transitionTimerFired(TimerBase*) {
  if (!m_opaque)
    setIsWanted(false);

  stopTimer();
}
