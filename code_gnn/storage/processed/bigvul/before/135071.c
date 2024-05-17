void AppCacheHost::LoadMainResourceCache(int64 cache_id) {
  DCHECK(cache_id != kAppCacheNoCacheId);
  if (pending_main_resource_cache_id_ == cache_id ||
      (main_resource_cache_.get() &&
       main_resource_cache_->cache_id() == cache_id)) {
    return;
  }
  pending_main_resource_cache_id_ = cache_id;
  storage()->LoadCache(cache_id, this);
}
