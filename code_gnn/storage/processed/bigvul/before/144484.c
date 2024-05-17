void WebContentsImpl::DidRedirectNavigation(
    NavigationHandle* navigation_handle) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidRedirectNavigation(navigation_handle));
}
