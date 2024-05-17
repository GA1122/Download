void DiscardableSharedMemoryManager::ReduceMemoryUsageUntilWithinMemoryLimit() {
  lock_.AssertAcquired();

  if (bytes_allocated_ <= memory_limit_)
    return;

  ReduceMemoryUsageUntilWithinLimit(memory_limit_);
  if (bytes_allocated_ > memory_limit_)
    ScheduleEnforceMemoryPolicy();
}
