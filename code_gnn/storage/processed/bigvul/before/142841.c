bool HTMLMediaElement::MediaTracksEnabledInternally() {
  return RuntimeEnabledFeatures::AudioVideoTracksEnabled() ||
         RuntimeEnabledFeatures::BackgroundVideoTrackOptimizationEnabled();
}
