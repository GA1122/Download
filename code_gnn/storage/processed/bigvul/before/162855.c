uint64_t GLManager::GenerateFenceSyncRelease() {
  return next_fence_sync_release_++;
}
