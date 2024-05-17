void HTMLMediaElement::loadTimerFired(TimerBase*) {
  if (m_pendingActionFlags & LoadTextTrackResource)
    honorUserPreferencesForAutomaticTextTrackSelection();

  if (m_pendingActionFlags & LoadMediaResource) {
    if (m_loadState == LoadingFromSourceElement)
      loadNextSourceChild();
    else
      loadInternal();
  }

  m_pendingActionFlags = 0;
}
