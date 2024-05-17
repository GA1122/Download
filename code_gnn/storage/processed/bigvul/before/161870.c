void DiscardableSharedMemoryManager::OnPurgeMemory() {
  base::AutoLock lock(lock_);
  ReduceMemoryUsageUntilWithinLimit(0);
}
