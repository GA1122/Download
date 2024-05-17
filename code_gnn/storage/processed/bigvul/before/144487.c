void WebContentsImpl::DidStartNavigationToPendingEntry(
      const GURL& url,
      NavigationController::ReloadType reload_type) {
  FOR_EACH_OBSERVER(
      WebContentsObserver,
      observers_,
      DidStartNavigationToPendingEntry(url, reload_type));
}
