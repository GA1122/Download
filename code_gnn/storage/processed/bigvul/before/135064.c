void FillCacheInfo(const AppCache* cache,
                   const GURL& manifest_url,
                   AppCacheStatus status, AppCacheInfo* info) {
  info->manifest_url = manifest_url;
  info->status = status;

  if (!cache)
    return;

  info->cache_id = cache->cache_id();

  if (!cache->is_complete())
    return;

  DCHECK(cache->owning_group());
  info->is_complete = true;
  info->group_id = cache->owning_group()->group_id();
  info->last_update_time = cache->update_time();
  info->creation_time = cache->owning_group()->creation_time();
  info->size = cache->cache_size();
}
