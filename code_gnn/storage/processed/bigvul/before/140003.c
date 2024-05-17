void HTMLMediaElement::invokeLoadAlgorithm() {
  BLINK_MEDIA_LOG << "invokeLoadAlgorithm(" << (void*)this << ")";

  stopPeriodicTimers();
  m_loadTimer.stop();
  cancelDeferredLoad();
  m_pendingActionFlags &= ~LoadMediaResource;
  m_sentStalledEvent = false;
  m_haveFiredLoadedData = false;
  m_displayMode = Unknown;

  m_loadState = WaitingForSource;
  m_currentSourceNode = nullptr;

  if (m_playPromiseResolveTaskHandle.isActive() &&
      !ScriptForbiddenScope::isScriptForbidden()) {
    m_playPromiseResolveTaskHandle.cancel();
    resolveScheduledPlayPromises();
  }
  if (m_playPromiseRejectTaskHandle.isActive() &&
      !ScriptForbiddenScope::isScriptForbidden()) {
    m_playPromiseRejectTaskHandle.cancel();
    rejectScheduledPlayPromises();
  }

  cancelPendingEventsAndCallbacks();

  if (m_networkState == kNetworkLoading || m_networkState == kNetworkIdle)
    scheduleEvent(EventTypeNames::abort);

  resetMediaPlayerAndMediaSource();

  if (m_networkState != kNetworkEmpty) {
    scheduleEvent(EventTypeNames::emptied);

    setNetworkState(kNetworkEmpty);

    forgetResourceSpecificTracks();

    m_readyState = kHaveNothing;
    m_readyStateMaximum = kHaveNothing;

    DCHECK(!m_paused || m_playPromiseResolvers.isEmpty());

    if (!m_paused) {
      m_paused = true;

      if (!ScriptForbiddenScope::isScriptForbidden()) {
        rejectPlayPromises(
            AbortError,
            "The play() request was interrupted by a new load request.");
      }
    }

    m_seeking = false;

    setOfficialPlaybackPosition(0);
    scheduleTimeupdateEvent(false);


    cueTimeline().updateActiveCues(0);
  } else if (!m_paused) {
    UseCounter::count(document(),
                      UseCounter::HTMLMediaElementLoadNetworkEmptyNotPaused);
  }

  setPlaybackRate(defaultPlaybackRate());

  m_error = nullptr;
  m_autoplaying = true;

  invokeResourceSelectionAlgorithm();

}
