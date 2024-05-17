scoped_refptr<EntryImpl> BackendImpl::OpenEntryImpl(const std::string& key) {
  if (disabled_)
    return NULL;

  TimeTicks start = TimeTicks::Now();
  uint32_t hash = base::Hash(key);
  Trace("Open hash 0x%x", hash);

  bool error;
  scoped_refptr<EntryImpl> cache_entry =
      MatchEntry(key, hash, false, Addr(), &error);
  if (cache_entry && ENTRY_NORMAL != cache_entry->entry()->Data()->state) {
    cache_entry = NULL;
    web_fonts_histogram::RecordEvictedEntry(key);
  } else if (!cache_entry) {
    web_fonts_histogram::RecordCacheMiss(key);
  }

  int current_size = data_->header.num_bytes / (1024 * 1024);
  int64_t total_hours = stats_.GetCounter(Stats::TIMER) / 120;
  int64_t no_use_hours = stats_.GetCounter(Stats::LAST_REPORT_TIMER) / 120;
  int64_t use_hours = total_hours - no_use_hours;

  if (!cache_entry) {
    stats_.OnEvent(Stats::OPEN_MISS);
    return NULL;
  }

  eviction_.OnOpenEntry(cache_entry.get());
  entry_count_++;

  Trace("Open hash 0x%x end: 0x%x", hash,
        cache_entry->entry()->address().value());
  CACHE_UMA(AGE_MS, "OpenTime", 0, start);
  CACHE_UMA(COUNTS_10000, "AllOpenBySize.Hit", 0, current_size);
  CACHE_UMA(HOURS, "AllOpenByTotalHours.Hit", 0,
            static_cast<base::HistogramBase::Sample>(total_hours));
  CACHE_UMA(HOURS, "AllOpenByUseHours.Hit", 0,
            static_cast<base::HistogramBase::Sample>(use_hours));
  stats_.OnEvent(Stats::OPEN_HIT);
  web_fonts_histogram::RecordCacheHit(cache_entry.get());
  return cache_entry;
}
