std::unique_ptr<HistogramSamples> SparseHistogram::SnapshotSamples() const {
  std::unique_ptr<SampleMap> snapshot(new SampleMap(name_hash()));

  base::AutoLock auto_lock(lock_);
  snapshot->Add(*samples_);
  return std::move(snapshot);
}
