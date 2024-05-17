void TextTrack::SetTrackList(TextTrackList* track_list) {
  if (!track_list && GetCueTimeline() && cues_)
    GetCueTimeline()->RemoveCues(this, cues_.Get());

  track_list_ = track_list;
  InvalidateTrackIndex();
}
