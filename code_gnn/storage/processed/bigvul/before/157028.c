bool MultibufferDataSource::GetSize(int64_t* size_out) {
  base::AutoLock auto_lock(lock_);
  if (total_bytes_ != kPositionNotSpecified) {
    *size_out = total_bytes_;
    return true;
  }
  *size_out = 0;
  return false;
}
