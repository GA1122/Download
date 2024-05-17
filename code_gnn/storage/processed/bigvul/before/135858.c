void CueTimeline::RemoveCue(TextTrack*, TextTrackCue* cue) {
  RemoveCueInternal(cue);
  UpdateActiveCues(MediaElement().currentTime());
}
