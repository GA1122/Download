void AppCacheHost::AssociateNoCache(const GURL& manifest_url) {
  AssociateCacheHelper(NULL, manifest_url);
}
