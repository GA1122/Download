void NavigationControllerImpl::Restore(
    int selected_navigation,
    RestoreType type,
    std::vector<NavigationEntry*>* entries) {
  DCHECK(GetEntryCount() == 0 && !GetPendingEntry());
  DCHECK(selected_navigation >= 0 &&
         selected_navigation < static_cast<int>(entries->size()));

  needs_reload_ = true;
  for (size_t i = 0; i < entries->size(); ++i) {
    NavigationEntryImpl* entry =
        NavigationEntryImpl::FromNavigationEntry((*entries)[i]);
    entries_.push_back(linked_ptr<NavigationEntryImpl>(entry));
  }
  entries->clear();

  FinishRestore(selected_navigation, type);
}
