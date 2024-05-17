void WebContentsImpl::ReadyToCommitNavigation(
    NavigationHandle* navigation_handle) {
  for (auto& observer : observers_)
    observer.ReadyToCommitNavigation(navigation_handle);
}
