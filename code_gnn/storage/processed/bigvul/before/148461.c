void WebContentsImpl::OnAppCacheAccessed(RenderViewHostImpl* source,
                                         const GURL& manifest_url,
                                         bool blocked_by_policy) {

  for (auto& observer : observers_)
    observer.AppCacheAccessed(manifest_url, blocked_by_policy);
}
