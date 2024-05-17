void HTMLMediaElement::textTrackModeChanged(TextTrack* track) {
  if (track->trackType() == TextTrack::TrackElement)
    track->setHasBeenConfigured(true);

  configureTextTrackDisplay();

  DCHECK(textTracks()->contains(track));
  textTracks()->scheduleChangeEvent();
}
