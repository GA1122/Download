void HTMLMediaElement::TextTrackModeChanged(TextTrack* track) {
  if (track->TrackType() == TextTrack::kTrackElement)
    track->SetHasBeenConfigured(true);

  ConfigureTextTrackDisplay();

  DCHECK(textTracks()->Contains(track));
  textTracks()->ScheduleChangeEvent();
}
