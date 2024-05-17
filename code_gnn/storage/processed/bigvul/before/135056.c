void AppCacheHost::AssociateCacheHelper(AppCache* cache,
                                        const GURL& manifest_url) {
  if (associated_cache_.get()) {
    associated_cache_->UnassociateHost(this);
  }

  associated_cache_ = cache;
  SetSwappableCache(cache ? cache->owning_group() : NULL);
  associated_cache_info_pending_ = cache && !cache->is_complete();
  AppCacheInfo info;
  if (cache)
    cache->AssociateHost(this);

  FillCacheInfo(cache, manifest_url, GetStatus(), &info);
  frontend_->OnCacheSelected(host_id_, info);
}
