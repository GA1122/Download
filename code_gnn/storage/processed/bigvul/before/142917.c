void HTMLMediaElement::StartPlaybackProgressTimer() {
  if (playback_progress_timer_.IsActive())
    return;

  previous_progress_time_ = WTF::CurrentTime();
  playback_progress_timer_.StartRepeating(kMaxTimeupdateEventFrequency,
                                          FROM_HERE);
}
