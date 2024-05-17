void AppCacheGroup::AddCache(AppCache* complete_cache) {
  DCHECK(complete_cache->is_complete());
  complete_cache->set_owning_group(this);

  if (!newest_complete_cache_) {
    newest_complete_cache_ = complete_cache;
    return;
  }

  if (complete_cache->IsNewerThan(newest_complete_cache_)) {
    old_caches_.push_back(newest_complete_cache_);
    newest_complete_cache_ = complete_cache;

    for (Caches::iterator it = old_caches_.begin(); it != old_caches_.end();
         ++it) {
      AppCache* cache = *it;
      for (AppCacheHost* host : cache->associated_hosts())
        host->SetSwappableCache(this);
    }
  } else {
    old_caches_.push_back(complete_cache);
  }
}
