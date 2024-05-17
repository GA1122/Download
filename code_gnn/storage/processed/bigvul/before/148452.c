void WebContentsImpl::NotifyNavigationEntryCommitted(
    const LoadCommittedDetails& load_details) {
  for (auto& observer : observers_)
    observer.NavigationEntryCommitted(load_details);
}
