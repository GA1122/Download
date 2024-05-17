void BackendImpl::OnStatsTimer() {
  if (disabled_)
    return;

  stats_.OnEvent(Stats::TIMER);
  int64_t time = stats_.GetCounter(Stats::TIMER);
  int64_t current = stats_.GetCounter(Stats::OPEN_ENTRIES);

  if (num_refs_ && (current != num_refs_)) {
    int64_t diff = (num_refs_ - current) / 50;
    if (!diff)
      diff = num_refs_ > current ? 1 : -1;
    current = current + diff;
    stats_.SetCounter(Stats::OPEN_ENTRIES, current);
    stats_.SetCounter(Stats::MAX_ENTRIES, max_refs_);
  }

  CACHE_UMA(COUNTS, "NumberOfReferences", 0, num_refs_);

  CACHE_UMA(COUNTS_10000, "EntryAccessRate", 0, entry_count_);
  CACHE_UMA(COUNTS, "ByteIORate", 0, byte_count_ / 1024);

  user_load_ = (entry_count_ > 300 || byte_count_ > 7 * 1024 * 1024);
  entry_count_ = 0;
  byte_count_ = 0;
  up_ticks_++;

  if (!data_)
    first_timer_ = false;
  if (first_timer_) {
    first_timer_ = false;
    if (ShouldReportAgain())
      ReportStats();
  }

  if (time % 10 == 0)
    StoreStats();
}
