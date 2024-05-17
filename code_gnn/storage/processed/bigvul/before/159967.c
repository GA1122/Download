void DiskCacheBackendTest::BackendEviction() {
  const int kMaxSize = 200 * 1024;
  const int kMaxEntryCount = 20;
  const int kWriteSize = kMaxSize / kMaxEntryCount;

  const int kWriteEntryCount = kMaxEntryCount * 2;

  static_assert(kWriteEntryCount * kWriteSize > kMaxSize,
                "must write more than MaxSize");

  SetMaxSize(kMaxSize);
  InitSparseCache(nullptr, nullptr);

  scoped_refptr<net::IOBuffer> buffer(new net::IOBuffer(kWriteSize));
  CacheTestFillBuffer(buffer->data(), kWriteSize, false);

  std::string key_prefix("prefix");
  for (int i = 0; i < kWriteEntryCount; ++i) {
    AddDelay();
    disk_cache::Entry* entry = NULL;
    ASSERT_THAT(CreateEntry(key_prefix + base::IntToString(i), &entry), IsOk());
    disk_cache::ScopedEntryPtr entry_closer(entry);
    EXPECT_EQ(kWriteSize,
              WriteData(entry, 1, 0, buffer.get(), kWriteSize, false));
  }

  int size = CalculateSizeOfAllEntries();
  EXPECT_GT(kMaxSize, size);
}
