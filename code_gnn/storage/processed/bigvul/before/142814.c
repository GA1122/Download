void HTMLMediaElement::HonorUserPreferencesForAutomaticTextTrackSelection() {
  if (!text_tracks_ || !text_tracks_->length())
    return;

  if (!should_perform_automatic_track_selection_)
    return;

  AutomaticTrackSelection::Configuration configuration;
  if (processing_preference_change_)
    configuration.disable_currently_enabled_tracks = true;
  if (text_tracks_visible_)
    configuration.force_enable_subtitle_or_caption_track = true;

  Settings* settings = GetDocument().GetSettings();
  if (settings) {
    configuration.text_track_kind_user_preference =
        settings->GetTextTrackKindUserPreference();
  }

  AutomaticTrackSelection track_selection(configuration);
  track_selection.Perform(*text_tracks_);
}
