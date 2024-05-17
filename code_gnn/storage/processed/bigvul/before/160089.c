void BackendImpl::SetType(net::CacheType type) {
  DCHECK_NE(net::MEMORY_CACHE, type);
  cache_type_ = type;
}
