void DiskCacheBackendTest::BackendEnumerations() {
  InitCache();
  Time initial = Time::Now();

  const int kNumEntries = 100;
  for (int i = 0; i < kNumEntries; i++) {
    std::string key = GenerateKey(true);
    disk_cache::Entry* entry;
    ASSERT_THAT(CreateEntry(key, &entry), IsOk());
    entry->Close();
  }
  EXPECT_EQ(kNumEntries, cache_->GetEntryCount());
  Time final = Time::Now();

  disk_cache::Entry* entry;
  std::unique_ptr<TestIterator> iter = CreateIterator();
  int count = 0;
  Time last_modified[kNumEntries];
  Time last_used[kNumEntries];
  while (iter->OpenNextEntry(&entry) == net::OK) {
    ASSERT_TRUE(NULL != entry);
    if (count < kNumEntries) {
      last_modified[count] = entry->GetLastModified();
      last_used[count] = entry->GetLastUsed();
      EXPECT_TRUE(initial <= last_modified[count]);
      EXPECT_TRUE(final >= last_modified[count]);
    }

    entry->Close();
    count++;
  };
  EXPECT_EQ(kNumEntries, count);

  iter = CreateIterator();
  count = 0;
  while (iter->OpenNextEntry(&entry) == net::OK) {
    ASSERT_TRUE(NULL != entry);
    if (count < kNumEntries) {
      EXPECT_TRUE(last_modified[count] == entry->GetLastModified());
      EXPECT_TRUE(last_used[count] == entry->GetLastUsed());
    }
    entry->Close();
    count++;
  };
  EXPECT_EQ(kNumEntries, count);
}
