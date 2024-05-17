void WebContentsImpl::GetManifest(const GetManifestCallback& callback) {
  manifest_manager_host_->GetManifest(callback);
}
