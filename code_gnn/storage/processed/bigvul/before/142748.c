void HTMLMediaElement::AutomaticTrackSelectionForUpdatedUserPreference() {
  if (!text_tracks_ || !text_tracks_->length())
    return;

  MarkCaptionAndSubtitleTracksAsUnconfigured();
  processing_preference_change_ = true;
  text_tracks_visible_ = false;
  HonorUserPreferencesForAutomaticTextTrackSelection();
  processing_preference_change_ = false;

  text_tracks_visible_ = text_tracks_->HasShowingTracks();
  UpdateTextTrackDisplay();
}
