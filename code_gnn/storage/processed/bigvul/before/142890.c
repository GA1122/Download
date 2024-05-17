void HTMLMediaElement::ScheduleTimeupdateEvent(bool periodic_event) {
  double media_time = CurrentPlaybackPosition();
  bool media_time_has_progressed =
      media_time != last_time_update_event_media_time_;

  if (periodic_event && !media_time_has_progressed)
    return;

  ScheduleEvent(event_type_names::kTimeupdate);

  last_time_update_event_media_time_ = media_time;

  if (!periodic_event && playback_progress_timer_.IsActive()) {
    playback_progress_timer_.StartRepeating(kMaxTimeupdateEventFrequency,
                                            FROM_HERE);
  }
}
