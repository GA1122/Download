PersistentSparseHistogramDataManager::GetSampleMapRecordsWhileLocked(
    uint64_t id) {
  lock_.AssertAcquired();

  auto found = sample_records_.find(id);
  if (found != sample_records_.end())
    return found->second.get();

  std::unique_ptr<PersistentSampleMapRecords>& samples = sample_records_[id];
  samples = std::make_unique<PersistentSampleMapRecords>(this, id);
  return samples.get();
}
