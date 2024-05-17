void AppCacheHost::LoadSelectedCache(int64 cache_id) {
  DCHECK(cache_id != kAppCacheNoCacheId);
  pending_selected_cache_id_ = cache_id;
  storage()->LoadCache(cache_id, this);
}
