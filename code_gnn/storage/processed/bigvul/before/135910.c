void TextTrackCueList::CollectActiveCues(TextTrackCueList& active_cues) const {
  active_cues.Clear();
  for (auto& cue : list_) {
    if (cue->IsActive())
      active_cues.Add(cue);
  }
 }
