TextTrackCue* TextTrackCueList::getCueById(const AtomicString& id) const {
  for (const auto& cue : list_) {
    if (cue->id() == id)
      return cue.Get();
  }
  return nullptr;
}
