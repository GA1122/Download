void HTMLMediaElement::ClearMediaPlayer() {
  ForgetResourceSpecificTracks();

  CloseMediaSource();

  CancelDeferredLoad();

  {
    AudioSourceProviderClientLockScope scope(*this);
    ClearMediaPlayerAndAudioSourceProviderClientWithoutLocking();
  }

  StopPeriodicTimers();
  load_timer_.Stop();

  pending_action_flags_ = 0;
  load_state_ = kWaitingForSource;

  playing_remotely_ = false;
  RemoteRouteAvailabilityChanged(WebRemotePlaybackAvailability::kUnknown);

  if (GetLayoutObject())
     GetLayoutObject()->SetShouldDoFullPaintInvalidation();
 }
