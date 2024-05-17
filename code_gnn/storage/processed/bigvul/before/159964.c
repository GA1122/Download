void DiskCacheBackendTest::BackendDoomRecent() {
  InitCache();

  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry("first", &entry), IsOk());
  entry->Close();
  ASSERT_THAT(CreateEntry("second", &entry), IsOk());
  entry->Close();
  FlushQueueForTest();

  AddDelay();
  Time middle = Time::Now();

  ASSERT_THAT(CreateEntry("third", &entry), IsOk());
  entry->Close();
  ASSERT_THAT(CreateEntry("fourth", &entry), IsOk());
  entry->Close();
  FlushQueueForTest();

  AddDelay();
  Time final = Time::Now();

  ASSERT_EQ(4, cache_->GetEntryCount());
  EXPECT_THAT(DoomEntriesSince(final), IsOk());
  ASSERT_EQ(4, cache_->GetEntryCount());

  EXPECT_THAT(DoomEntriesSince(middle), IsOk());
  ASSERT_EQ(2, cache_->GetEntryCount());

  ASSERT_THAT(OpenEntry("second", &entry), IsOk());
  entry->Close();
}
