int BackendImpl::MaxFileSize() const {
  return cache_type() == net::PNACL_CACHE ? max_size_ : max_size_ / 8;
}
