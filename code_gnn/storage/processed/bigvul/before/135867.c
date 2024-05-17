void TextTrack::CueWillChange(TextTrackCue* cue) {
  if (GetCueTimeline())
    GetCueTimeline()->RemoveCue(this, cue);
}
