void NavigationControllerImpl::LoadEntry(NavigationEntryImpl* entry) {
  SetPendingEntry(entry);
  NavigateToPendingEntry(NO_RELOAD);
}
