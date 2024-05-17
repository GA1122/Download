void HTMLMediaElement::FinishSeek() {
  BLINK_MEDIA_LOG << "finishSeek(" << (void*)this << ")";

  seeking_ = false;

  SetOfficialPlaybackPosition(CurrentPlaybackPosition());

  ScheduleTimeupdateEvent(false);

  ScheduleEvent(event_type_names::kSeeked);

  SetDisplayMode(kVideo);
}
