void HTMLMediaElement::InvokeLoadAlgorithm() {
  BLINK_MEDIA_LOG << "invokeLoadAlgorithm(" << (void*)this << ")";

  StopPeriodicTimers();
  load_timer_.Stop();
  CancelDeferredLoad();
  pending_action_flags_ &= ~kLoadMediaResource;
  sent_stalled_event_ = false;
  have_fired_loaded_data_ = false;
  display_mode_ = kUnknown;

  autoplay_policy_->StopAutoplayMutedWhenVisible();

  load_state_ = kWaitingForSource;
  current_source_node_ = nullptr;

  if (play_promise_resolve_task_handle_.IsActive() &&
      !ScriptForbiddenScope::IsScriptForbidden()) {
    play_promise_resolve_task_handle_.Cancel();
    ResolveScheduledPlayPromises();
  }
  if (play_promise_reject_task_handle_.IsActive() &&
      !ScriptForbiddenScope::IsScriptForbidden()) {
    play_promise_reject_task_handle_.Cancel();
    RejectScheduledPlayPromises();
  }

  CancelPendingEventsAndCallbacks();

  if (network_state_ == kNetworkLoading || network_state_ == kNetworkIdle)
    ScheduleEvent(event_type_names::kAbort);

  ResetMediaPlayerAndMediaSource();

  if (network_state_ != kNetworkEmpty) {
    ScheduleEvent(event_type_names::kEmptied);

    SetNetworkState(kNetworkEmpty);

    ForgetResourceSpecificTracks();

    ready_state_ = kHaveNothing;
    ready_state_maximum_ = kHaveNothing;

    DCHECK(!paused_ || play_promise_resolvers_.IsEmpty());

    if (!paused_) {
      paused_ = true;

      RecordPlayPromiseRejected(PlayPromiseRejectReason::kInterruptedByLoad);
      RejectPlayPromises(DOMExceptionCode::kAbortError,
                         "The play() request was interrupted by a new load "
                         "request. https://goo.gl/LdLk22");
    }

    seeking_ = false;

    SetOfficialPlaybackPosition(0);
    ScheduleTimeupdateEvent(false);


    GetCueTimeline().UpdateActiveCues(0);
  } else if (!paused_) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kHTMLMediaElementLoadNetworkEmptyNotPaused);
  }

  setPlaybackRate(defaultPlaybackRate());

  error_ = nullptr;
  can_autoplay_ = true;

  InvokeResourceSelectionAlgorithm();

}
