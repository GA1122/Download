void TextTrackCue::setPauseOnExit(bool value) {
  if (pause_on_exit_ == value)
    return;

  CueWillChange();
  pause_on_exit_ = value;
  CueDidChange();
}
