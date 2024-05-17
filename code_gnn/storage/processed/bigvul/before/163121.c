std::unique_ptr<disk_cache::Backend> CreateInMemoryDiskCache() {
  std::unique_ptr<disk_cache::Backend> cache;
  net::TestCompletionCallback callback;
  int rv = disk_cache::CreateCacheBackend(
      net::MEMORY_CACHE, net::CACHE_BACKEND_DEFAULT, base::FilePath(), 0, false,
      nullptr, &cache, callback.callback());
  EXPECT_EQ(net::OK, callback.GetResult(rv));

  return cache;
}
