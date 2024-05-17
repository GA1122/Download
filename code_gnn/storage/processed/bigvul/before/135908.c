TextTrackCue* TextTrackCueList::AnonymousIndexedGetter(unsigned index) const {
  if (index < list_.size())
    return list_[index].Get();
  return nullptr;
}
