void CueTimeline::AddCue(TextTrack* track, TextTrackCue* cue) {
  DCHECK_NE(track->mode(), TextTrack::DisabledKeyword());
  AddCueInternal(cue);
  UpdateActiveCues(MediaElement().currentTime());
}
