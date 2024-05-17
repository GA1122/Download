bool PersistentSparseHistogramDataManager::LoadRecords(
    PersistentSampleMapRecords* sample_map_records) {
  base::AutoLock auto_lock(lock_);
  bool found = false;

  if (!sample_map_records->found_.empty()) {
    sample_map_records->records_.reserve(sample_map_records->records_.size() +
                                         sample_map_records->found_.size());
    sample_map_records->records_.insert(sample_map_records->records_.end(),
                                        sample_map_records->found_.begin(),
                                        sample_map_records->found_.end());
    sample_map_records->found_.clear();
    found = true;
  }

  const int kMinimumNumberToLoad = 10;
  const uint64_t match_id = sample_map_records->sample_map_id_;

  for (int count = 0; !found || count < kMinimumNumberToLoad; ++count) {
    uint64_t found_id;
    PersistentMemoryAllocator::Reference ref =
        PersistentSampleMap::GetNextPersistentRecord(record_iterator_,
                                                     &found_id);

    if (!ref)
      break;

    if (found_id == match_id) {
      sample_map_records->records_.push_back(ref);
      found = true;
    } else {
      PersistentSampleMapRecords* samples =
          GetSampleMapRecordsWhileLocked(found_id);
      DCHECK(samples);
      samples->found_.push_back(ref);
    }
  }

  return found;
}
