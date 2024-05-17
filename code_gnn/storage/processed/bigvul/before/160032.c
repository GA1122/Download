void BackendImpl::FirstEviction() {
  DCHECK(data_->header.create_time);
  if (!GetEntryCount())
    return;   

  Time create_time = Time::FromInternalValue(data_->header.create_time);
  CACHE_UMA(AGE, "FillupAge", 0, create_time);

  int64_t use_time = stats_.GetCounter(Stats::TIMER);
  CACHE_UMA(HOURS, "FillupTime", 0, static_cast<int>(use_time / 120));
  CACHE_UMA(PERCENTAGE, "FirstHitRatio", 0, stats_.GetHitRatio());

  if (!use_time)
    use_time = 1;
  CACHE_UMA(COUNTS_10000, "FirstEntryAccessRate", 0,
            static_cast<int>(data_->header.num_entries / use_time));
  CACHE_UMA(COUNTS, "FirstByteIORate", 0,
            static_cast<int>((data_->header.num_bytes / 1024) / use_time));

  int avg_size = data_->header.num_bytes / GetEntryCount();
  CACHE_UMA(COUNTS, "FirstEntrySize", 0, avg_size);

  int large_entries_bytes = stats_.GetLargeEntriesSize();
  int large_ratio = large_entries_bytes * 100 / data_->header.num_bytes;
  CACHE_UMA(PERCENTAGE, "FirstLargeEntriesRatio", 0, large_ratio);

  if (new_eviction_) {
    CACHE_UMA(PERCENTAGE, "FirstResurrectRatio", 0, stats_.GetResurrectRatio());
    CACHE_UMA(PERCENTAGE, "FirstNoUseRatio", 0,
              data_->header.lru.sizes[0] * 100 / data_->header.num_entries);
    CACHE_UMA(PERCENTAGE, "FirstLowUseRatio", 0,
              data_->header.lru.sizes[1] * 100 / data_->header.num_entries);
    CACHE_UMA(PERCENTAGE, "FirstHighUseRatio", 0,
              data_->header.lru.sizes[2] * 100 / data_->header.num_entries);
  }

  stats_.ResetRatios();
}
