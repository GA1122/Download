void RenderProcessImpl::ClearTransportDIBCache() {
  for (size_t i = 0; i < arraysize(shared_mem_cache_); ++i) {
    if (shared_mem_cache_[i]) {
      FreeTransportDIB(shared_mem_cache_[i]);
      shared_mem_cache_[i] = NULL;
    }
  }
}
