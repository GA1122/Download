void HTMLMediaElement::setReadyState(ReadyState state) {
  BLINK_MEDIA_LOG << "setReadyState(" << (void*)this << ", "
                  << static_cast<int>(state) << ") - current state is "
                  << static_cast<int>(m_readyState);

  bool wasPotentiallyPlaying = potentiallyPlaying();

  ReadyState oldState = m_readyState;
  ReadyState newState = state;

  bool tracksAreReady = textTracksAreReady();

  if (newState == oldState && m_tracksAreReady == tracksAreReady)
    return;

  m_tracksAreReady = tracksAreReady;

  if (tracksAreReady) {
    m_readyState = newState;
  } else {
    if (newState <= kHaveMetadata)
      m_readyState = newState;
    else
      m_readyState = kHaveCurrentData;
  }

  if (oldState > m_readyStateMaximum)
    m_readyStateMaximum = oldState;

  if (m_networkState == kNetworkEmpty)
    return;

  if (m_seeking) {
    if (wasPotentiallyPlaying && m_readyState < kHaveFutureData)
      scheduleEvent(EventTypeNames::waiting);

    if (m_readyState >= kHaveCurrentData)
      finishSeek();
  } else {
    if (wasPotentiallyPlaying && m_readyState < kHaveFutureData) {
      setOfficialPlaybackPosition(currentPlaybackPosition());

      scheduleTimeupdateEvent(false);
      scheduleEvent(EventTypeNames::waiting);
    }
  }

  if (m_readyState >= kHaveMetadata && oldState < kHaveMetadata) {
    createPlaceholderTracksIfNecessary();

    selectInitialTracksIfNecessary();

    MediaFragmentURIParser fragmentParser(m_currentSrc);
    m_fragmentEndTime = fragmentParser.endTime();

    setOfficialPlaybackPosition(earliestPossiblePosition());

    m_duration = m_webMediaPlayer->duration();
    scheduleEvent(EventTypeNames::durationchange);

    if (isHTMLVideoElement())
      scheduleEvent(EventTypeNames::resize);
    scheduleEvent(EventTypeNames::loadedmetadata);

    bool jumped = false;
    if (m_defaultPlaybackStartPosition > 0) {
      seek(m_defaultPlaybackStartPosition);
      jumped = true;
    }
    m_defaultPlaybackStartPosition = 0;

    double initialPlaybackPosition = fragmentParser.startTime();
    if (std::isnan(initialPlaybackPosition))
      initialPlaybackPosition = 0;

    if (!jumped && initialPlaybackPosition > 0) {
      UseCounter::count(document(),
                        UseCounter::HTMLMediaElementSeekToFragmentStart);
      seek(initialPlaybackPosition);
      jumped = true;
    }

    if (layoutObject())
      layoutObject()->updateFromElement();
  }

  bool shouldUpdateDisplayState = false;

  if (m_readyState >= kHaveCurrentData && oldState < kHaveCurrentData &&
      !m_haveFiredLoadedData) {
    setOfficialPlaybackPosition(currentPlaybackPosition());

    m_haveFiredLoadedData = true;
    shouldUpdateDisplayState = true;
    scheduleEvent(EventTypeNames::loadeddata);
    setShouldDelayLoadEvent(false);
  }

  bool isPotentiallyPlaying = potentiallyPlaying();
  if (m_readyState == kHaveFutureData && oldState <= kHaveCurrentData &&
      tracksAreReady) {
    scheduleEvent(EventTypeNames::canplay);
    if (isPotentiallyPlaying)
      scheduleNotifyPlaying();
    shouldUpdateDisplayState = true;
  }

  if (m_readyState == kHaveEnoughData && oldState < kHaveEnoughData &&
      tracksAreReady) {
    if (oldState <= kHaveCurrentData) {
      scheduleEvent(EventTypeNames::canplay);
      if (isPotentiallyPlaying)
        scheduleNotifyPlaying();
    }

    if (shouldAutoplay()) {
      m_autoplayUmaHelper->onAutoplayInitiated(AutoplaySource::Attribute);

      if (!isGestureNeededForPlayback()) {
        if (isGestureNeededForPlaybackIfCrossOriginExperimentEnabled()) {
          m_autoplayUmaHelper->recordCrossOriginAutoplayResult(
              CrossOriginAutoplayResult::AutoplayBlocked);
        } else {
          m_autoplayUmaHelper->recordCrossOriginAutoplayResult(
              CrossOriginAutoplayResult::AutoplayAllowed);
        }
        if (isHTMLVideoElement() && muted() &&
            RuntimeEnabledFeatures::autoplayMutedVideosEnabled()) {
          if (!m_autoplayVisibilityObserver) {
            m_autoplayVisibilityObserver = new ElementVisibilityObserver(
                this,
                WTF::bind(&HTMLMediaElement::onVisibilityChangedForAutoplay,
                          wrapWeakPersistent(this)));
            m_autoplayVisibilityObserver->start();
          }
        } else {
          m_paused = false;
          scheduleEvent(EventTypeNames::play);
          scheduleNotifyPlaying();
          m_autoplaying = false;
        }
      } else {
        m_autoplayUmaHelper->recordCrossOriginAutoplayResult(
            CrossOriginAutoplayResult::AutoplayBlocked);
      }
    }

    scheduleEvent(EventTypeNames::canplaythrough);

    shouldUpdateDisplayState = true;
  }

  if (shouldUpdateDisplayState)
    updateDisplayState();

  updatePlayState();
  cueTimeline().updateActiveCues(currentTime());
}
