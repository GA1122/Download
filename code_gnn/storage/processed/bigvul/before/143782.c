PersistentMemoryAllocator::Reference PersistentSampleMapRecords::CreateNew(
    HistogramBase::Sample value) {
  return PersistentSampleMap::CreatePersistentRecord(data_manager_->allocator_,
                                                     sample_map_id_, value);
}
