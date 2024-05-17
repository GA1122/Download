int TextTrack::TrackIndexRelativeToRenderedTracks() {
  DCHECK(track_list_);

  if (rendered_track_index_ == kInvalidTrackIndex)
    rendered_track_index_ =
        track_list_->GetTrackIndexRelativeToRenderedTracks(this);

  return rendered_track_index_;
}
