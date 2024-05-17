void WebContentsImpl::DidStartNavigation(NavigationHandle* navigation_handle) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidStartNavigation(navigation_handle));
}
