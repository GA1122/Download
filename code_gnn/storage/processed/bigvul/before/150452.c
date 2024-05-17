NavigationEntryImpl* NavigationControllerImpl::GetEntryAtIndex(
    int index) const {
  if (index < 0 || index >= GetEntryCount())
    return nullptr;

  return entries_[index].get();
}
