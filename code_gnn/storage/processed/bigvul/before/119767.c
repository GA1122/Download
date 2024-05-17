NavigationEntry* NavigationControllerImpl::GetEntryAtOffset(
    int offset) const {
  int index = GetIndexForOffset(offset);
  if (index < 0 || index >= GetEntryCount())
    return NULL;

  return entries_[index].get();
}
