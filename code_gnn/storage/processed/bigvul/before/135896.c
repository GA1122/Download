unsigned TextTrackCue::CueIndex() {
  DCHECK(track() && track()->cues());
  TextTrackCueList* cue_list = track()->cues();
  if (!cue_list->IsCueIndexValid(cue_index_))
    cue_list->ValidateCueIndexes();
  return cue_index_;
}
