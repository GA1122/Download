void DiskCacheBackendTest::BackendFixEnumerators() {
  InitCache();

  int seed = static_cast<int>(Time::Now().ToInternalValue());
  srand(seed);

  const int kNumEntries = 10;
  for (int i = 0; i < kNumEntries; i++) {
    std::string key = GenerateKey(true);
    disk_cache::Entry* entry;
    ASSERT_THAT(CreateEntry(key, &entry), IsOk());
    entry->Close();
  }
  EXPECT_EQ(kNumEntries, cache_->GetEntryCount());

  disk_cache::Entry *entry1, *entry2;
  std::unique_ptr<TestIterator> iter1 = CreateIterator(),
                                iter2 = CreateIterator();
  ASSERT_THAT(iter1->OpenNextEntry(&entry1), IsOk());
  ASSERT_TRUE(NULL != entry1);
  entry1->Close();
  entry1 = NULL;

  for (int i = 0; i < kNumEntries / 2; i++) {
    if (entry1)
      entry1->Close();
    ASSERT_THAT(iter1->OpenNextEntry(&entry1), IsOk());
    ASSERT_TRUE(NULL != entry1);

    ASSERT_THAT(iter2->OpenNextEntry(&entry2), IsOk());
    ASSERT_TRUE(NULL != entry2);
    entry2->Close();
  }

  entry1->Doom();
  ASSERT_THAT(iter2->OpenNextEntry(&entry2), IsOk());
  ASSERT_TRUE(NULL != entry2);

  EXPECT_NE(entry2->GetKey(), entry1->GetKey());
  entry1->Close();
  entry2->Close();

  ASSERT_THAT(iter2->OpenNextEntry(&entry2), IsOk());
  ASSERT_TRUE(NULL != entry2);
  entry2->Close();
}
