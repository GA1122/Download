bool HTMLMediaElement::HasSelectedVideoTrack() {
  DCHECK(RuntimeEnabledFeatures::BackgroundVideoTrackOptimizationEnabled());

  return video_tracks_ && video_tracks_->selectedIndex() != -1;
}
