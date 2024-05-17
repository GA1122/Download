void HTMLMediaElement::PauseInternal() {
  BLINK_MEDIA_LOG << "pauseInternal(" << (void*)this << ")";

  if (network_state_ == kNetworkEmpty)
    InvokeResourceSelectionAlgorithm();

  can_autoplay_ = false;

  if (!paused_) {
    paused_ = true;
    ScheduleTimeupdateEvent(false);
    ScheduleEvent(event_type_names::kPause);

    SetOfficialPlaybackPosition(CurrentPlaybackPosition());

    ScheduleRejectPlayPromises(DOMExceptionCode::kAbortError);
  }

  UpdatePlayState();
}
