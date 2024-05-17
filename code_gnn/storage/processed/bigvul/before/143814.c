PersistentSparseHistogramDataManager::UseSampleMapRecords(uint64_t id,
                                                          const void* user) {
  base::AutoLock auto_lock(lock_);
  return GetSampleMapRecordsWhileLocked(id)->Acquire(user);
}
