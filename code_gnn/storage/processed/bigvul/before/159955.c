void DiskCacheBackendTest::BackendCalculateSizeOfEntriesBetween() {
  InitCache();

  EXPECT_EQ(0, CalculateSizeOfEntriesBetween(base::Time(), base::Time::Max()));

  Time start = Time::Now();

  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry("first", &entry), IsOk());
  entry->Close();
  FlushQueueForTest();

  AddDelay();
  Time middle = Time::Now();
  AddDelay();

  ASSERT_THAT(CreateEntry("second", &entry), IsOk());
  entry->Close();
  ASSERT_THAT(CreateEntry("third_entry", &entry), IsOk());
  entry->Close();
  FlushQueueForTest();

  AddDelay();
  Time end = Time::Now();

  int size_1 = GetRoundedSize(GetEntryMetadataSize("first"));
  int size_2 = GetRoundedSize(GetEntryMetadataSize("second"));
  int size_3 = GetRoundedSize(GetEntryMetadataSize("third_entry"));

  ASSERT_EQ(3, cache_->GetEntryCount());
  ASSERT_EQ(CalculateSizeOfAllEntries(),
            CalculateSizeOfEntriesBetween(base::Time(), base::Time::Max()));

  int start_end = CalculateSizeOfEntriesBetween(start, end);
  ASSERT_EQ(CalculateSizeOfAllEntries(), start_end);
  ASSERT_EQ(size_1 + size_2 + size_3, start_end);

  ASSERT_EQ(size_1, CalculateSizeOfEntriesBetween(start, middle));
  ASSERT_EQ(size_2 + size_3, CalculateSizeOfEntriesBetween(middle, end));

  ASSERT_THAT(DoomAllEntries(), IsOk());
  EXPECT_EQ(0, CalculateSizeOfEntriesBetween(base::Time(), base::Time::Max()));
}
