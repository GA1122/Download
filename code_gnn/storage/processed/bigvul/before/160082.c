void BackendImpl::ReportStats() {
  CACHE_UMA(COUNTS, "Entries", 0, data_->header.num_entries);

  int current_size = data_->header.num_bytes / (1024 * 1024);
  int max_size = max_size_ / (1024 * 1024);
  int hit_ratio_as_percentage = stats_.GetHitRatio();

  CACHE_UMA(COUNTS_10000, "Size2", 0, current_size);
  if (base::RandInt(0, 99) < hit_ratio_as_percentage)
    CACHE_UMA(COUNTS_10000, "HitRatioBySize2", 0, current_size);
  CACHE_UMA(COUNTS_10000, "MaxSize2", 0, max_size);
  if (!max_size)
    max_size++;
  CACHE_UMA(PERCENTAGE, "UsedSpace", 0, current_size * 100 / max_size);

  CACHE_UMA(COUNTS_10000, "AverageOpenEntries2", 0,
            static_cast<int>(stats_.GetCounter(Stats::OPEN_ENTRIES)));
  CACHE_UMA(COUNTS_10000, "MaxOpenEntries2", 0,
            static_cast<int>(stats_.GetCounter(Stats::MAX_ENTRIES)));
  stats_.SetCounter(Stats::MAX_ENTRIES, 0);

  CACHE_UMA(COUNTS_10000, "TotalFatalErrors", 0,
            static_cast<int>(stats_.GetCounter(Stats::FATAL_ERROR)));
  CACHE_UMA(COUNTS_10000, "TotalDoomCache", 0,
            static_cast<int>(stats_.GetCounter(Stats::DOOM_CACHE)));
  CACHE_UMA(COUNTS_10000, "TotalDoomRecentEntries", 0,
            static_cast<int>(stats_.GetCounter(Stats::DOOM_RECENT)));
  stats_.SetCounter(Stats::FATAL_ERROR, 0);
  stats_.SetCounter(Stats::DOOM_CACHE, 0);
  stats_.SetCounter(Stats::DOOM_RECENT, 0);

  int age = (Time::Now() -
             Time::FromInternalValue(data_->header.create_time)).InHours();
  if (age)
    CACHE_UMA(HOURS, "FilesAge", 0, age);

  int64_t total_hours = stats_.GetCounter(Stats::TIMER) / 120;
  if (!data_->header.create_time || !data_->header.lru.filled) {
    int cause = data_->header.create_time ? 0 : 1;
    if (!data_->header.lru.filled)
      cause |= 2;
    CACHE_UMA(CACHE_ERROR, "ShortReport", 0, cause);
    CACHE_UMA(HOURS, "TotalTimeNotFull", 0, static_cast<int>(total_hours));
    return;
  }


  CACHE_UMA(HOURS, "TotalTime", 0, static_cast<int>(total_hours));
  if (base::RandInt(0, 99) < hit_ratio_as_percentage)
    CACHE_UMA(HOURS, "HitRatioByTotalTime", 0, static_cast<int>(total_hours));

  int64_t use_hours = stats_.GetCounter(Stats::LAST_REPORT_TIMER) / 120;
  stats_.SetCounter(Stats::LAST_REPORT_TIMER, stats_.GetCounter(Stats::TIMER));

  if (use_hours)
    use_hours = total_hours - use_hours;

  if (!use_hours || !GetEntryCount() || !data_->header.num_bytes)
    return;

  CACHE_UMA(HOURS, "UseTime", 0, static_cast<int>(use_hours));
  if (base::RandInt(0, 99) < hit_ratio_as_percentage)
    CACHE_UMA(HOURS, "HitRatioByUseTime", 0, static_cast<int>(use_hours));
  CACHE_UMA(PERCENTAGE, "HitRatio", 0, hit_ratio_as_percentage);

  int64_t trim_rate = stats_.GetCounter(Stats::TRIM_ENTRY) / use_hours;
  CACHE_UMA(COUNTS, "TrimRate", 0, static_cast<int>(trim_rate));

  int avg_size = data_->header.num_bytes / GetEntryCount();
  CACHE_UMA(COUNTS, "EntrySize", 0, avg_size);
  CACHE_UMA(COUNTS, "EntriesFull", 0, data_->header.num_entries);

  CACHE_UMA(PERCENTAGE, "IndexLoad", 0,
            data_->header.num_entries * 100 / (mask_ + 1));

  int large_entries_bytes = stats_.GetLargeEntriesSize();
  int large_ratio = large_entries_bytes * 100 / data_->header.num_bytes;
  CACHE_UMA(PERCENTAGE, "LargeEntriesRatio", 0, large_ratio);

  if (new_eviction_) {
    CACHE_UMA(PERCENTAGE, "ResurrectRatio", 0, stats_.GetResurrectRatio());
    CACHE_UMA(PERCENTAGE, "NoUseRatio", 0,
              data_->header.lru.sizes[0] * 100 / data_->header.num_entries);
    CACHE_UMA(PERCENTAGE, "LowUseRatio", 0,
              data_->header.lru.sizes[1] * 100 / data_->header.num_entries);
    CACHE_UMA(PERCENTAGE, "HighUseRatio", 0,
              data_->header.lru.sizes[2] * 100 / data_->header.num_entries);
    CACHE_UMA(PERCENTAGE, "DeletedRatio", 0,
              data_->header.lru.sizes[4] * 100 / data_->header.num_entries);
  }

  stats_.ResetRatios();
  stats_.SetCounter(Stats::TRIM_ENTRY, 0);

  if (cache_type_ == net::DISK_CACHE)
    block_files_.ReportStats();
}
