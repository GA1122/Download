void WebContentsImpl::NotifyNavigationEntryChanged(
    const EntryChangedDetails& change_details) {
  for (auto& observer : observers_)
    observer.NavigationEntryChanged(change_details);
}
