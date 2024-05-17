void HTMLMediaElement::ScheduleNextSourceChild() {
  pending_action_flags_ |= kLoadMediaResource;
  load_timer_.StartOneShot(TimeDelta(), FROM_HERE);
}
