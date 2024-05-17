void CueTimeline::RemoveCues(TextTrack*, const TextTrackCueList* cues) {
  for (size_t i = 0; i < cues->length(); ++i)
    RemoveCueInternal(cues->AnonymousIndexedGetter(i));
  UpdateActiveCues(MediaElement().currentTime());
}
