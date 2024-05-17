bool SharedMemoryHandleProvider::InitForSize(size_t size) {
#if DCHECK_IS_ON()
  DCHECK_EQ(map_ref_count_, 0);
#endif
  DCHECK(!shared_memory_);
  shared_memory_.emplace();
  if (shared_memory_->CreateAnonymous(size)) {
    mapped_size_ = size;
    read_only_flag_ = false;
    return true;
  }
  return false;
}
