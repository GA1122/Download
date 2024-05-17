void HTMLMediaElement::FinishSeek() {
  BLINK_MEDIA_LOG << "finishSeek(" << (void*)this << ")";

  seeking_ = false;

  SetOfficialPlaybackPosition(CurrentPlaybackPosition());

  ScheduleTimeupdateEvent(false);

  ScheduleEvent(EventTypeNames::seeked);

  SetDisplayMode(kVideo);
}
