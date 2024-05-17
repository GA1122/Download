void HTMLMediaElement::PlayInternal() {
  BLINK_MEDIA_LOG << "playInternal(" << (void*)this << ")";

  if (network_state_ == kNetworkEmpty)
    InvokeResourceSelectionAlgorithm();

  if (EndedPlayback(LoopCondition::kIgnored))
    Seek(0);

  if (paused_) {
    paused_ = false;
    ScheduleEvent(EventTypeNames::play);

    if (ready_state_ <= kHaveCurrentData)
      ScheduleEvent(EventTypeNames::waiting);
    else if (ready_state_ >= kHaveFutureData)
      ScheduleNotifyPlaying();
  } else if (ready_state_ >= kHaveFutureData) {
    ScheduleResolvePlayPromises();
  }

  can_autoplay_ = false;

  SetIgnorePreloadNone();
  UpdatePlayState();
}
