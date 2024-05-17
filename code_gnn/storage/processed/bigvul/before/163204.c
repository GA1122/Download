void NavigationControllerImpl::NotifyEntryChanged(
    const NavigationEntry* entry) {
  EntryChangedDetails det;
  det.changed_entry = entry;
  det.index = GetIndexOfEntry(
      NavigationEntryImpl::FromNavigationEntry(entry));
  delegate_->NotifyNavigationEntryChanged(det);
}
