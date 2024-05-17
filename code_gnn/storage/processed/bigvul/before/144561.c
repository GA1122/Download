void WebContentsImpl::HasManifest(const HasManifestCallback& callback) {
  manifest_manager_host_->HasManifest(GetMainFrame(), callback);
}
