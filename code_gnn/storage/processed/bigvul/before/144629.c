void WebContentsImpl::ReadyToCommitNavigation(
    NavigationHandle* navigation_handle) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    ReadyToCommitNavigation(navigation_handle));
}
