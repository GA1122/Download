void Resource::CachedMetadataHandlerImpl::ClearCachedMetadata(
    CachedMetadataHandler::CacheType cache_type) {
  cached_metadata_ = nullptr;
  if (cache_type == CachedMetadataHandler::kSendToPlatform)
    SendToPlatform();
}
