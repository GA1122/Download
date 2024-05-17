void AppCacheHost::OnCacheLoaded(AppCache* cache, int64 cache_id) {
  if (cache_id == pending_main_resource_cache_id_) {
    pending_main_resource_cache_id_ = kAppCacheNoCacheId;
    main_resource_cache_ = cache;
  } else if (cache_id == pending_selected_cache_id_) {
    pending_selected_cache_id_ = kAppCacheNoCacheId;
    FinishCacheSelection(cache, NULL);
  }
}
