void WebContentsImpl::DidStartNavigation(NavigationHandle* navigation_handle) {
  for (auto& observer : observers_)
    observer.DidStartNavigation(navigation_handle);
}
