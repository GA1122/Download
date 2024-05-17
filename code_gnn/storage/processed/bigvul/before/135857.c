void CueTimeline::HideCues(TextTrack*, const TextTrackCueList* cues) {
  for (size_t i = 0; i < cues->length(); ++i)
    cues->AnonymousIndexedGetter(i)->RemoveDisplayTree();
}
