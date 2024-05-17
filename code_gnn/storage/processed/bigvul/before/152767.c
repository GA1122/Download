bool SparseHistogram::AddSamplesFromPickle(PickleIterator* iter) {
  base::AutoLock auto_lock(lock_);
  return samples_->AddFromPickle(iter);
}
