size_t TextTrackCueList::FindInsertionIndex(
    const TextTrackCue* cue_to_insert) const {
  auto it =
      std::upper_bound(list_.begin(), list_.end(), cue_to_insert, CueIsBefore);
  size_t index = SafeCast<size_t>(it - list_.begin());
  SECURITY_DCHECK(index <= list_.size());
  return index;
}
