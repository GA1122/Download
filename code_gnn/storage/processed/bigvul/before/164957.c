void RecordCacheFlags(HistogramCacheFlag flag) {
  UMA_HISTOGRAM_ENUMERATION("Navigation.BackForward.CacheFlags", flag,
                            HISTOGRAM_CACHE_FLAG_MAX);
}
