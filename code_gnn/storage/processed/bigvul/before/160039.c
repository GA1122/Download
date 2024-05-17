int32_t BackendImpl::GetEntryCount() const {
  if (!index_.get() || disabled_)
    return 0;
  int32_t not_deleted =
      data_->header.num_entries - data_->header.lru.sizes[Rankings::DELETED];

  if (not_deleted < 0) {
    NOTREACHED();
    not_deleted = 0;
  }

  return not_deleted;
}
