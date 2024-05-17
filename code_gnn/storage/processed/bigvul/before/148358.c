void WebContentsImpl::DidStartNavigationToPendingEntry(const GURL& url,
                                                       ReloadType reload_type) {
  for (auto& observer : observers_)
    observer.DidStartNavigationToPendingEntry(url, reload_type);
}
