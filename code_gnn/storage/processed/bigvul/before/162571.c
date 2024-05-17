void Resource::CachedMetadataHandlerImpl::SetCachedMetadata(
    uint32_t data_type_id,
    const char* data,
    size_t size,
    CachedMetadataHandler::CacheType cache_type) {
  DCHECK(!cached_metadata_);
  cached_metadata_ = CachedMetadata::Create(data_type_id, data, size);
  if (cache_type == CachedMetadataHandler::kSendToPlatform)
    SendToPlatform();
}
