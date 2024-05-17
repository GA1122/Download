HTMLMediaElement::HTMLMediaElement(const QualifiedName& tagName,
                                   Document& document)
    : HTMLElement(tagName, document),
      SuspendableObject(&document),
      m_loadTimer(TaskRunnerHelper::get(TaskType::Unthrottled, &document),
                  this,
                  &HTMLMediaElement::loadTimerFired),
      m_progressEventTimer(
          TaskRunnerHelper::get(TaskType::Unthrottled, &document),
          this,
          &HTMLMediaElement::progressEventTimerFired),
      m_playbackProgressTimer(
          TaskRunnerHelper::get(TaskType::Unthrottled, &document),
          this,
          &HTMLMediaElement::playbackProgressTimerFired),
      m_audioTracksTimer(
          TaskRunnerHelper::get(TaskType::Unthrottled, &document),
          this,
          &HTMLMediaElement::audioTracksTimerFired),
      m_viewportFillDebouncerTimer(
          TaskRunnerHelper::get(TaskType::Unthrottled, &document),
          this,
          &HTMLMediaElement::viewportFillDebouncerTimerFired),
      m_checkViewportIntersectionTimer(
          TaskRunnerHelper::get(TaskType::Unthrottled, &document),
          this,
          &HTMLMediaElement::checkViewportIntersectionTimerFired),
      m_playedTimeRanges(),
      m_asyncEventQueue(GenericEventQueue::create(this)),
      m_playbackRate(1.0f),
      m_defaultPlaybackRate(1.0f),
      m_networkState(kNetworkEmpty),
      m_readyState(kHaveNothing),
      m_readyStateMaximum(kHaveNothing),
      m_volume(1.0f),
      m_lastSeekTime(0),
      m_previousProgressTime(std::numeric_limits<double>::max()),
      m_duration(std::numeric_limits<double>::quiet_NaN()),
      m_lastTimeUpdateEventWallTime(0),
      m_lastTimeUpdateEventMediaTime(std::numeric_limits<double>::quiet_NaN()),
      m_defaultPlaybackStartPosition(0),
      m_loadState(WaitingForSource),
      m_deferredLoadState(NotDeferred),
      m_deferredLoadTimer(this, &HTMLMediaElement::deferredLoadTimerFired),
      m_webLayer(nullptr),
      m_displayMode(Unknown),
      m_officialPlaybackPosition(0),
      m_officialPlaybackPositionNeedsUpdate(true),
      m_fragmentEndTime(std::numeric_limits<double>::quiet_NaN()),
      m_pendingActionFlags(0),
      m_lockedPendingUserGesture(false),
      m_lockedPendingUserGestureIfCrossOriginExperimentEnabled(true),
      m_playing(false),
      m_shouldDelayLoadEvent(false),
      m_haveFiredLoadedData(false),
      m_autoplaying(true),
      m_muted(false),
      m_paused(true),
      m_seeking(false),
      m_sentStalledEvent(false),
      m_ignorePreloadNone(false),
      m_textTracksVisible(false),
      m_shouldPerformAutomaticTrackSelection(true),
      m_tracksAreReady(true),
      m_processingPreferenceChange(false),
      m_playingRemotely(false),
      m_inOverlayFullscreenVideo(false),
      m_mostlyFillingViewport(false),
      m_audioTracks(this, AudioTrackList::create(*this)),
      m_videoTracks(this, VideoTrackList::create(*this)),
      m_textTracks(this, nullptr),
      m_audioSourceNode(nullptr),
      m_autoplayUmaHelper(AutoplayUmaHelper::create(this)),
      m_remotePlaybackClient(nullptr),
      m_autoplayVisibilityObserver(nullptr),
      m_mediaControls(nullptr) {
  BLINK_MEDIA_LOG << "HTMLMediaElement(" << (void*)this << ")";

  m_lockedPendingUserGesture = computeLockedPendingUserGesture(document);
  m_lockedPendingUserGestureIfCrossOriginExperimentEnabled =
      isDocumentCrossOrigin(document);

  LocalFrame* frame = document.frame();
  if (frame) {
    m_remotePlaybackClient =
        frame->loader().client()->createWebRemotePlaybackClient(*this);
  }

  setHasCustomStyleCallbacks();
  addElementToDocumentMap(this, &document);

  UseCounter::count(document, UseCounter::HTMLMediaElement);
}
