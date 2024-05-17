NavigationEntryImpl* NavigationControllerImpl::GetEntryAtOffset(
    int offset) const {
  return GetEntryAtIndex(GetIndexForOffset(offset));
}
