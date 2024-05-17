Resource* ResourceFetcher::CachedResource(const KURL& resource_url) const {
  KURL url = MemoryCache::RemoveFragmentIdentifierIfNeeded(resource_url);
  const WeakMember<Resource>& resource = cached_resources_map_.at(url);
  return resource.Get();
}
