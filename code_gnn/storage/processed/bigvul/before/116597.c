void RenderProcessImpl::ReleaseTransportDIB(TransportDIB* mem) {
  if (PutSharedMemInCache(mem)) {
    shared_mem_cache_cleaner_.Reset();
    return;
  }

  FreeTransportDIB(mem);
}
