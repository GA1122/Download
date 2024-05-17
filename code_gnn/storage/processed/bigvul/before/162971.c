void ManifestManager::RequestManifestImpl(
    InternalRequestManifestCallback callback) {
  if (!may_have_manifest_) {
    std::move(callback).Run(GURL(), Manifest(), nullptr);
    return;
  }

  if (!manifest_dirty_) {
    std::move(callback).Run(manifest_url_, manifest_,
                            manifest_debug_info_.get());
    return;
  }

  pending_callbacks_.push_back(std::move(callback));

  if (pending_callbacks_.size() > 1)
    return;

  FetchManifest();
}
