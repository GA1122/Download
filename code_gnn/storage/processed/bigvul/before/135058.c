void AppCacheHost::AssociateIncompleteCache(AppCache* cache,
                                            const GURL& manifest_url) {
  DCHECK(cache && !cache->is_complete());
  DCHECK(!manifest_url.is_empty());
  AssociateCacheHelper(cache, manifest_url);
}
