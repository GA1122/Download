int BackendImpl::SyncCalculateSizeOfAllEntries() {
  DCHECK_NE(net::APP_CACHE, cache_type_);
  if (disabled_)
    return net::ERR_FAILED;

  return data_->header.num_bytes;
}
