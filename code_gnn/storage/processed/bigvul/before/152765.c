void SparseHistogram::AddCount(Sample value, int count) {
  if (count <= 0) {
    NOTREACHED();
    return;
  }
  {
    base::AutoLock auto_lock(lock_);
    samples_->Accumulate(value, count);
  }

  FindAndRunCallback(value);
}
