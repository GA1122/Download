void TextTrack::InvalidateTrackIndex() {
  track_index_ = kInvalidTrackIndex;
  rendered_track_index_ = kInvalidTrackIndex;
}
