  int64_t AppCacheDatabase::GetOriginUsage(const url::Origin& origin) {
  std::vector<CacheRecord> records;
  if (!FindCachesForOrigin(origin, &records))
//   std::vector<CacheRecord> caches;
//   if (!FindCachesForOrigin(origin, &caches))
      return 0;
  
    int64_t origin_usage = 0;
  for (const auto& record : records)
    origin_usage += record.cache_size;
//   for (const auto& cache : caches)
//     origin_usage += cache.cache_size + cache.padding_size;
    return origin_usage;
  }