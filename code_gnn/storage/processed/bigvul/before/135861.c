static bool TrackIndexCompare(TextTrack* a, TextTrack* b) {
  return a->TrackIndex() - b->TrackIndex() < 0;
}
