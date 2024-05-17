void Resource::OnPurgeMemory() {
  Prune();
  if (!cache_handler_)
    return;
  cache_handler_->ClearCachedMetadata(CachedMetadataHandler::kCacheLocally);
}
