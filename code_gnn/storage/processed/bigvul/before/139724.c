String ResourceFetcher::GetCacheIdentifier() const {
  if (Context().IsControlledByServiceWorker())
    return String::Number(Context().ServiceWorkerID());
  return MemoryCache::DefaultCacheIdentifier();
}
