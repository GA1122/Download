void WebContentsImpl::NotifyManifestUrlChanged(
    const base::Optional<GURL>& manifest_url) {
  for (auto& observer : observers_)
    observer.DidUpdateWebManifestURL(manifest_url);
}
