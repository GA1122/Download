Resource::CachedMetadataHandlerImpl::GetCachedMetadata(
    uint32_t data_type_id) const {
  if (!cached_metadata_ || cached_metadata_->DataTypeID() != data_type_id)
    return nullptr;
  return cached_metadata_;
}
