int TextTrack::TrackIndex() {
  DCHECK(track_list_);

  if (track_index_ == kInvalidTrackIndex)
    track_index_ = track_list_->GetTrackIndex(this);

  return track_index_;
}
