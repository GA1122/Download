void ManifestManager::ResolveCallbacks(ResolveState state) {
  if (state == ResolveStateFailure)
    manifest_ = Manifest();

  manifest_dirty_ = state != ResolveStateSuccess;

  std::vector<InternalRequestManifestCallback> callbacks;
  swap(callbacks, pending_callbacks_);

  for (auto& callback : callbacks) {
    std::move(callback).Run(manifest_url_, manifest_,
                            manifest_debug_info_.get());
  }
}
