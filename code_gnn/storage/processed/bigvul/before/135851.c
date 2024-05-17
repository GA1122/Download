void CueTimeline::AddCues(TextTrack* track, const TextTrackCueList* cues) {
  DCHECK_NE(track->mode(), TextTrack::DisabledKeyword());
  for (size_t i = 0; i < cues->length(); ++i)
    AddCueInternal(cues->AnonymousIndexedGetter(i));
  UpdateActiveCues(MediaElement().currentTime());
}
