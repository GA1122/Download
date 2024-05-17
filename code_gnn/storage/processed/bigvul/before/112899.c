void RunGetCacheEntryCallback(
    const GDataCache::GetCacheEntryCallback& callback,
    bool* success,
    GDataCache::CacheEntry* cache_entry) {
  DCHECK(success);
  DCHECK(cache_entry);

  if (!callback.is_null())
    callback.Run(*success, *cache_entry);
}
