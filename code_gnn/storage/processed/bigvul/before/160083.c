void BackendImpl::RestartCache(bool failure) {
  int64_t errors = stats_.GetCounter(Stats::FATAL_ERROR);
  int64_t full_dooms = stats_.GetCounter(Stats::DOOM_CACHE);
  int64_t partial_dooms = stats_.GetCounter(Stats::DOOM_RECENT);
  int64_t last_report = stats_.GetCounter(Stats::LAST_REPORT);

  PrepareForRestart();
  if (failure) {
    DCHECK(!num_refs_);
    DCHECK(open_entries_.empty());
    DelayedCacheCleanup(path_);
  } else {
    DeleteCache(path_, false);
  }

  if (unit_test_) {
    init_ = true;   
  } else if (SyncInit() == net::OK) {
    stats_.SetCounter(Stats::FATAL_ERROR, errors);
    stats_.SetCounter(Stats::DOOM_CACHE, full_dooms);
    stats_.SetCounter(Stats::DOOM_RECENT, partial_dooms);
    stats_.SetCounter(Stats::LAST_REPORT, last_report);
  }
}
