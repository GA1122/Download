std::unique_ptr<disk_cache::BackendImpl> CreateExistingEntryCache(
    const base::FilePath& cache_path) {
  net::TestCompletionCallback cb;

  std::unique_ptr<disk_cache::BackendImpl> cache(
      std::make_unique<disk_cache::BackendImpl>(cache_path,
                                                  nullptr,
                                                  nullptr,
                                                  nullptr));
  int rv = cache->Init(cb.callback());
  if (cb.GetResult(rv) != net::OK)
    return std::unique_ptr<disk_cache::BackendImpl>();

  disk_cache::Entry* entry = NULL;
  rv = cache->CreateEntry(kExistingEntryKey, &entry, cb.callback());
  if (cb.GetResult(rv) != net::OK)
    return std::unique_ptr<disk_cache::BackendImpl>();
  entry->Close();

  return cache;
}
