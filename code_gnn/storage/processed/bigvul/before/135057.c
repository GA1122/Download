void AppCacheHost::AssociateCompleteCache(AppCache* cache) {
  DCHECK(cache && cache->is_complete());
  AssociateCacheHelper(cache, cache->owning_group()->manifest_url());
}
