TextTrackCueList* TextTrack::cues() {
  if (mode_ != DisabledKeyword())
    return EnsureTextTrackCueList();
  return nullptr;
}
