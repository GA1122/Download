void HTMLMediaElement::PlayInternal() {
  BLINK_MEDIA_LOG << "playInternal(" << (void*)this << ")";

  if (lazy_load_visibility_observer_) {
    lazy_load_visibility_observer_->Stop();
    lazy_load_visibility_observer_ = nullptr;
  }

  if (network_state_ == kNetworkEmpty)
    InvokeResourceSelectionAlgorithm();

  if (EndedPlayback(LoopCondition::kIgnored))
    Seek(0);

  if (paused_) {
    paused_ = false;
    ScheduleEvent(event_type_names::kPlay);

    if (ready_state_ <= kHaveCurrentData)
      ScheduleEvent(event_type_names::kWaiting);
    else if (ready_state_ >= kHaveFutureData)
      ScheduleNotifyPlaying();
  } else if (ready_state_ >= kHaveFutureData) {
    ScheduleResolvePlayPromises();
  }

  can_autoplay_ = false;

  SetIgnorePreloadNone();
  UpdatePlayState();
}
