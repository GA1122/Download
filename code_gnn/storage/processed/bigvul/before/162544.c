bool Resource::HasCacheControlNoStoreHeader() const {
  return GetResponse().CacheControlContainsNoStore() ||
         GetResourceRequest().CacheControlContainsNoStore();
}
