std::unique_ptr<HistogramBase> SparseHistogram::PersistentCreate(
    PersistentHistogramAllocator* allocator,
    const std::string& name,
    HistogramSamples::Metadata* meta,
    HistogramSamples::Metadata* logged_meta) {
  return WrapUnique(
      new SparseHistogram(allocator, name, meta, logged_meta));
}
