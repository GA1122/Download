const AppCacheNamespace* AppCache::FindNamespace(
    const std::vector<AppCacheNamespace>& namespaces,
    const GURL& url) {
  size_t count = namespaces.size();
  for (size_t i = 0; i < count; ++i) {
    if (namespaces[i].IsMatch(url))
      return &namespaces[i];
  }
  return nullptr;
}
