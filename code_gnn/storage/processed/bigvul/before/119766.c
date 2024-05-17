NavigationEntry* NavigationControllerImpl::GetEntryAtIndex(
    int index) const {
  return entries_.at(index).get();
}
