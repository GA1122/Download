void HTMLMediaElement::honorUserPreferencesForAutomaticTextTrackSelection() {
  if (!m_textTracks || !m_textTracks->length())
    return;

  if (!m_shouldPerformAutomaticTrackSelection)
    return;

  AutomaticTrackSelection::Configuration configuration;
  if (m_processingPreferenceChange)
    configuration.disableCurrentlyEnabledTracks = true;
  if (m_textTracksVisible)
    configuration.forceEnableSubtitleOrCaptionTrack = true;

  Settings* settings = document().settings();
  if (settings) {
    configuration.textTrackKindUserPreference =
        settings->getTextTrackKindUserPreference();
  }

  AutomaticTrackSelection trackSelection(configuration);
  trackSelection.perform(*m_textTracks);
}
