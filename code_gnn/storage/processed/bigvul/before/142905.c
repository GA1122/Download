void HTMLMediaElement::SetReadyState(ReadyState state) {
  BLINK_MEDIA_LOG << "setReadyState(" << (void*)this << ", "
                  << static_cast<int>(state) << ") - current state is "
                  << static_cast<int>(ready_state_);

  bool was_potentially_playing = PotentiallyPlaying();

  ReadyState old_state = ready_state_;
  ReadyState new_state = state;

  bool tracks_are_ready = TextTracksAreReady();

  if (new_state == old_state && tracks_are_ready_ == tracks_are_ready)
    return;

  tracks_are_ready_ = tracks_are_ready;

  if (tracks_are_ready) {
    ready_state_ = new_state;
  } else {
    if (new_state <= kHaveMetadata)
      ready_state_ = new_state;
    else
      ready_state_ = kHaveCurrentData;
  }

  if (old_state > ready_state_maximum_)
    ready_state_maximum_ = old_state;

  if (network_state_ == kNetworkEmpty)
    return;

  if (seeking_) {
    if (was_potentially_playing && ready_state_ < kHaveFutureData)
      ScheduleEvent(event_type_names::kWaiting);

    if (ready_state_ >= kHaveCurrentData)
      FinishSeek();
  } else {
    if (was_potentially_playing && ready_state_ < kHaveFutureData) {
      SetOfficialPlaybackPosition(CurrentPlaybackPosition());

      ScheduleTimeupdateEvent(false);
      ScheduleEvent(event_type_names::kWaiting);
    }
  }

  if (ready_state_ >= kHaveMetadata && old_state < kHaveMetadata) {
    CreatePlaceholderTracksIfNecessary();

    MediaFragmentURIParser fragment_parser(current_src_);
    fragment_end_time_ = fragment_parser.EndTime();

    SetOfficialPlaybackPosition(EarliestPossiblePosition());

    duration_ = web_media_player_->Duration();
    ScheduleEvent(event_type_names::kDurationchange);

    if (IsHTMLVideoElement())
      ScheduleEvent(event_type_names::kResize);
    ScheduleEvent(event_type_names::kLoadedmetadata);

    bool jumped = false;
    if (default_playback_start_position_ > 0) {
      Seek(default_playback_start_position_);
      jumped = true;
    }
    default_playback_start_position_ = 0;

    double initial_playback_position = fragment_parser.StartTime();
    if (std::isnan(initial_playback_position))
      initial_playback_position = 0;

    if (!jumped && initial_playback_position > 0) {
      UseCounter::Count(GetDocument(),
                        WebFeature::kHTMLMediaElementSeekToFragmentStart);
      Seek(initial_playback_position);
      jumped = true;
    }

    if (GetLayoutObject())
      GetLayoutObject()->UpdateFromElement();
  }

  bool should_update_display_state = false;

  bool is_potentially_playing = PotentiallyPlaying();
  if (ready_state_ >= kHaveCurrentData && old_state < kHaveCurrentData &&
      !have_fired_loaded_data_) {
    SetOfficialPlaybackPosition(CurrentPlaybackPosition());

    have_fired_loaded_data_ = true;
    should_update_display_state = true;
    ScheduleEvent(event_type_names::kLoadeddata);
    SetShouldDelayLoadEvent(false);

    if (IsHTMLVideoElement() && web_media_player_->DidLazyLoad() &&
        !is_potentially_playing) {
      lazy_load_visibility_observer_ =
          MakeGarbageCollected<ElementVisibilityObserver>(
              this, WTF::BindRepeating(
                        &HTMLMediaElement::OnVisibilityChangedForLazyLoad,
                        WrapWeakPersistent(this)));
      lazy_load_visibility_observer_->Start();
    }
  }

  if (ready_state_ == kHaveFutureData && old_state <= kHaveCurrentData &&
      tracks_are_ready) {
    ScheduleEvent(event_type_names::kCanplay);
    if (is_potentially_playing)
      ScheduleNotifyPlaying();
    should_update_display_state = true;
  }

  if (ready_state_ == kHaveEnoughData && old_state < kHaveEnoughData &&
      tracks_are_ready) {
    if (old_state <= kHaveCurrentData) {
      ScheduleEvent(event_type_names::kCanplay);
      if (is_potentially_playing)
        ScheduleNotifyPlaying();
    }

    if (autoplay_policy_->RequestAutoplayByAttribute()) {
      paused_ = false;
      ScheduleEvent(event_type_names::kPlay);
      ScheduleNotifyPlaying();
      can_autoplay_ = false;
    }

    ScheduleEvent(event_type_names::kCanplaythrough);

    should_update_display_state = true;
  }

  if (should_update_display_state)
    UpdateDisplayState();

  UpdatePlayState();
  GetCueTimeline().UpdateActiveCues(currentTime());
}
