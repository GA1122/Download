void HTMLMediaElement::LoadTimerFired(TimerBase*) {
  if (pending_action_flags_ & kLoadTextTrackResource)
    HonorUserPreferencesForAutomaticTextTrackSelection();

  if (pending_action_flags_ & kLoadMediaResource) {
    if (load_state_ == kLoadingFromSourceElement)
      LoadNextSourceChild();
    else
      LoadInternal();
  }

  pending_action_flags_ = 0;
}
