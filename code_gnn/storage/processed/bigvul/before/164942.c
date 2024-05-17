void LogBackForwardNavigationFlagsHistogram(int load_flags) {
  if (load_flags & net::LOAD_VALIDATE_CACHE)
    RecordCacheFlags(HISTOGRAM_VALIDATE_CACHE);

  if (load_flags & net::LOAD_BYPASS_CACHE)
    RecordCacheFlags(HISTOGRAM_BYPASS_CACHE);

  if (load_flags & net::LOAD_SKIP_CACHE_VALIDATION)
    RecordCacheFlags(HISTOGRAM_SKIP_CACHE_VALIDATION);

  if (load_flags & net::LOAD_ONLY_FROM_CACHE)
    RecordCacheFlags(HISTOGRAM_ONLY_FROM_CACHE);

  if (load_flags & net::LOAD_DISABLE_CACHE)
    RecordCacheFlags(HISTOGRAM_DISABLE_CACHE);
}
