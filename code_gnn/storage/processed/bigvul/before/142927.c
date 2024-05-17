void HTMLMediaElement::TextTrackReadyStateChanged(TextTrack* track) {
  if (GetWebMediaPlayer() &&
      text_tracks_when_resource_selection_began_.Contains(track)) {
    if (track->GetReadinessState() != TextTrack::kLoading) {
      SetReadyState(
          static_cast<ReadyState>(GetWebMediaPlayer()->GetReadyState()));
    }
  } else {
    if (GetMediaControls() &&
        track->GetReadinessState() == TextTrack::kFailedToLoad) {
      GetMediaControls()->OnTrackElementFailedToLoad();
    }
  }
}
