bool BackendImpl::IsLoaded() const {
  CACHE_UMA(COUNTS, "PendingIO", 0, num_pending_io_);
  if (user_flags_ & kNoLoadProtection)
    return false;

  return (num_pending_io_ > 5 || user_load_);
}
