bool TextTrackCueList::Remove(TextTrackCue* cue) {
  size_t index = list_.Find(cue);
  if (index == kNotFound)
    return false;

  list_.EraseAt(index);
  InvalidateCueIndex(index);
  cue->InvalidateCueIndex();
  return true;
}
