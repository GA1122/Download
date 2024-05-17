TextTrackCueList* TextTrack::EnsureTextTrackCueList() {
  if (!cues_) {
    cues_ = TextTrackCueList::Create();
  }

  return cues_.Get();
}
