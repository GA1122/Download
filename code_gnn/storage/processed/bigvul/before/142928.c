bool HTMLMediaElement::TextTracksAreReady() const {
  for (const auto& text_track : text_tracks_when_resource_selection_began_) {
    if (text_track->GetReadinessState() == TextTrack::kLoading ||
        text_track->GetReadinessState() == TextTrack::kNotLoaded)
      return false;
  }

  return true;
}
