void HTMLMediaElement::Seek(double time) {
  BLINK_MEDIA_LOG << "seek(" << (void*)this << ", " << time << ")";

  if (!web_media_player_ || ready_state_ == kHaveNothing)
    return;

  SetIgnorePreloadNone();

  double now = currentTime();


  seeking_ = true;

  time = std::min(time, duration());

  time = std::max(time, EarliestPossiblePosition());

  double media_time = GetWebMediaPlayer()->MediaTimeForTimeValue(time);
  if (time != media_time) {
    BLINK_MEDIA_LOG << "seek(" << (void*)this << ", " << time
                    << ") - media timeline equivalent is " << media_time;
    time = media_time;
  }

  TimeRanges* seekable_ranges = seekable();

  if (!seekable_ranges->length()) {
    seeking_ = false;
    return;
  }
  time = seekable_ranges->Nearest(time, now);

  if (playing_ && last_seek_time_ < now)
    AddPlayedRange(last_seek_time_, now);

  last_seek_time_ = time;

  ScheduleEvent(event_type_names::kSeeking);

  GetWebMediaPlayer()->Seek(time);

}
