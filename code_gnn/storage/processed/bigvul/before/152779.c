SparseHistogram::SparseHistogram(PersistentHistogramAllocator* allocator,
                                 const std::string& name,
                                 HistogramSamples::Metadata* meta,
                                 HistogramSamples::Metadata* logged_meta)
    : HistogramBase(name),
      samples_(new PersistentSampleMap(HashMetricName(name), allocator, meta)),
      logged_samples_(
          new PersistentSampleMap(samples_->id() + 1, allocator, logged_meta)) {
}
