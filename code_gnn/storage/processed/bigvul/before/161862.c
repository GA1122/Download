size_t DiscardableSharedMemoryManager::GetBytesAllocated() {
  base::AutoLock lock(lock_);

  return bytes_allocated_;
}
