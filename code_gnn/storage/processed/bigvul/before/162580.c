void Resource::CachedMetadataHandlerImpl::SetSerializedCachedMetadata(
    const char* data,
    size_t size) {
  DCHECK(!cached_metadata_);
  cached_metadata_ = CachedMetadata::CreateFromSerializedData(data, size);
}
