int BrowserContextIOData::GetMaxCacheSizeHint() const {
  int max_cache_size_hint = GetSharedData().max_cache_size_hint;
  static int upper_limit = std::numeric_limits<int>::max() / (1024 * 1024);
  DCHECK_LE(max_cache_size_hint, upper_limit);
  return max_cache_size_hint;
}
