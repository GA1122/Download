bool hasDuplicateLabel(TextTrack* currentTrack) {
  DCHECK(currentTrack);
  TextTrackList* trackList = currentTrack->trackList();
  String currentTrackLabel = currentTrack->label();
  for (unsigned i = 0; i < trackList->length(); i++) {
    TextTrack* track = trackList->anonymousIndexedGetter(i);
    if (currentTrack != track && currentTrackLabel == track->label())
      return true;
  }
  return false;
}
