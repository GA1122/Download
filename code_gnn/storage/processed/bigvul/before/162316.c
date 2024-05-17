SharedMemoryHandleProvider::~SharedMemoryHandleProvider() {
  base::AutoLock lock(mapping_lock_);

#if DCHECK_IS_ON()
  DCHECK_EQ(map_ref_count_, 0);
#endif

  if (shared_memory_ && shared_memory_->memory()) {
    DVLOG(3) << __func__ << ": Unmapping memory for in-process access @"
             << shared_memory_->memory() << '.';
    CHECK(shared_memory_->Unmap());
  }
}
