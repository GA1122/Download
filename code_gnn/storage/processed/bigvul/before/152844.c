void MetricsWebContentsObserver::ReadyToCommitNavigation(
    content::NavigationHandle* navigation_handle) {
  if (committed_load_)
    committed_load_->ReadyToCommitNavigation(navigation_handle);
}
