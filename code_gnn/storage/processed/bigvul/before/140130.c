void HTMLMediaElement::textTrackReadyStateChanged(TextTrack* track) {
  if (webMediaPlayer() &&
      m_textTracksWhenResourceSelectionBegan.contains(track)) {
    if (track->getReadinessState() != TextTrack::Loading)
      setReadyState(static_cast<ReadyState>(webMediaPlayer()->getReadyState()));
  } else {
    if (mediaControls() &&
        track->getReadinessState() == TextTrack::FailedToLoad) {
      mediaControls()->onTrackElementFailedToLoad();
    }
  }
}
