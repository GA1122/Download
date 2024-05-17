std::unique_ptr<HistogramSamples> Histogram::SnapshotDelta() {
  DCHECK(!final_delta_created_);

  std::unique_ptr<HistogramSamples> snapshot = SnapshotSampleVector();
  if (!logged_samples_) {
    logged_samples_.swap(snapshot);
    return SnapshotSampleVector();
  }

  snapshot->Subtract(*logged_samples_);
  logged_samples_->Add(*snapshot);
  return snapshot;
}
