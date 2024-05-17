void HTMLMediaElement::StartProgressEventTimer() {
  if (progress_event_timer_.IsActive())
    return;

  previous_progress_time_ = WTF::CurrentTime();
  progress_event_timer_.StartRepeating(TimeDelta::FromMilliseconds(350),
                                       FROM_HERE);
}
