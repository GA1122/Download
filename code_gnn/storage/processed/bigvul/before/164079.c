AppCacheEntry* AppCache::GetEntry(const GURL& url) {
  auto it = entries_.find(url);
  return (it != entries_.end()) ? &(it->second) : nullptr;
}
