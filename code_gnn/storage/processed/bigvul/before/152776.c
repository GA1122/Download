std::unique_ptr<HistogramSamples> SparseHistogram::SnapshotFinalDelta() const {
  DCHECK(!final_delta_created_);
  final_delta_created_ = true;

  std::unique_ptr<SampleMap> snapshot(new SampleMap(name_hash()));
  base::AutoLock auto_lock(lock_);
  snapshot->Add(*samples_);

  snapshot->Subtract(*logged_samples_);
  return std::move(snapshot);
}
