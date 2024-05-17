void DiskCacheBackendTest::BackendLoad() {
  InitCache();
  int seed = static_cast<int>(Time::Now().ToInternalValue());
  srand(seed);

  disk_cache::Entry* entries[kLargeNumEntries];
  for (int i = 0; i < kLargeNumEntries; i++) {
    std::string key = GenerateKey(true);
    ASSERT_THAT(CreateEntry(key, &entries[i]), IsOk());
  }
  EXPECT_EQ(kLargeNumEntries, cache_->GetEntryCount());

  for (int i = 0; i < kLargeNumEntries; i++) {
    int source1 = rand() % kLargeNumEntries;
    int source2 = rand() % kLargeNumEntries;
    disk_cache::Entry* temp = entries[source1];
    entries[source1] = entries[source2];
    entries[source2] = temp;
  }

  for (int i = 0; i < kLargeNumEntries; i++) {
    disk_cache::Entry* entry;
    ASSERT_THAT(OpenEntry(entries[i]->GetKey(), &entry), IsOk());
    EXPECT_TRUE(entry == entries[i]);
    entry->Close();
    entries[i]->Doom();
    entries[i]->Close();
  }
  FlushQueueForTest();
  EXPECT_EQ(0, cache_->GetEntryCount());
}
