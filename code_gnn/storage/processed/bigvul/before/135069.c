AppCacheStatus AppCacheHost::GetStatus() {
  AppCache* cache = associated_cache();
  if (!cache)
    return APPCACHE_STATUS_UNCACHED;

  if (!cache->owning_group())
    return APPCACHE_STATUS_DOWNLOADING;

  if (cache->owning_group()->is_obsolete())
    return APPCACHE_STATUS_OBSOLETE;
  if (cache->owning_group()->update_status() == AppCacheGroup::CHECKING)
    return APPCACHE_STATUS_CHECKING;
  if (cache->owning_group()->update_status() == AppCacheGroup::DOWNLOADING)
    return APPCACHE_STATUS_DOWNLOADING;
  if (swappable_cache_.get())
    return APPCACHE_STATUS_UPDATE_READY;
  return APPCACHE_STATUS_IDLE;
}
