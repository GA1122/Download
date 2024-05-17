WebMediaPlayer::TrackId HTMLMediaElement::getSelectedVideoTrackId() {
  DCHECK(RuntimeEnabledFeatures::backgroundVideoTrackOptimizationEnabled());
  DCHECK(hasSelectedVideoTrack());

  int selectedTrackIndex = m_videoTracks->selectedIndex();
  VideoTrack* track = m_videoTracks->anonymousIndexedGetter(selectedTrackIndex);
  return track->id();
}
