PersistentSampleMapRecords* PersistentHistogramAllocator::UseSampleMapRecords(
    uint64_t id,
    const void* user) {
  return sparse_histogram_data_manager_.UseSampleMapRecords(id, user);
}
