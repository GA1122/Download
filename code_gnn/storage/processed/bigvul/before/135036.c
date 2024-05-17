bool AppCacheUpdateJob::ShouldSkipUrlFetch(const AppCacheEntry& entry) {
  if (entry.IsExplicit() || entry.IsFallback() || entry.IsIntercept())
    return false;

  return false;
}
