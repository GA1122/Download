bool RenderProcessImpl::PutSharedMemInCache(TransportDIB* mem) {
  const int slot = FindFreeCacheSlot(mem->size());
  if (slot == -1)
    return false;

  shared_mem_cache_[slot] = mem;
  return true;
}
