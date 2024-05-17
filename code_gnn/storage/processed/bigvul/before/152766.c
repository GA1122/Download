void SparseHistogram::AddSamples(const HistogramSamples& samples) {
  base::AutoLock auto_lock(lock_);
  samples_->Add(samples);
}
