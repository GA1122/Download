void WebContentsImpl::NotifyNavigationListPruned(
    const PrunedDetails& pruned_details) {
  for (auto& observer : observers_)
    observer.NavigationListPruned(pruned_details);
}
