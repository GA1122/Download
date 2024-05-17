void TextTrackCueList::UpdateCueIndex(TextTrackCue* cue) {
  if (!Remove(cue))
    return;
  Add(cue);
}
