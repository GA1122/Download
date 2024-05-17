void HTMLMediaElement::TimeChanged() {
  BLINK_MEDIA_LOG << "timeChanged(" << (void*)this << ")";

  GetCueTimeline().UpdateActiveCues(currentTime());

  if (seeking_ && ready_state_ >= kHaveCurrentData &&
      !GetWebMediaPlayer()->Seeking())
    FinishSeek();

  double now = CurrentPlaybackPosition();
  double dur = duration();

  if (!std::isnan(dur) && dur && now >= dur &&
      GetDirectionOfPlayback() == kForward) {
    if (Loop()) {
      Seek(EarliestPossiblePosition());
    } else {
      ScheduleTimeupdateEvent(false);

      if (!paused_) {
        paused_ = true;
        ScheduleEvent(event_type_names::kPause);
        ScheduleRejectPlayPromises(DOMExceptionCode::kAbortError);
      }
      ScheduleEvent(event_type_names::kEnded);
    }
  }
  UpdatePlayState();
}
