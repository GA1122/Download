std::unique_ptr<HistogramSamples> SparseHistogram::SnapshotDelta() {
  DCHECK(!final_delta_created_);

  std::unique_ptr<SampleMap> snapshot(new SampleMap(name_hash()));
  base::AutoLock auto_lock(lock_);
  snapshot->Add(*samples_);

  snapshot->Subtract(*logged_samples_);
  logged_samples_->Add(*snapshot);
  return std::move(snapshot);
}
