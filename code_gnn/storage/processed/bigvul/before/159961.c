void DiskCacheBackendTest::BackendDoomAll2() {
  EXPECT_EQ(2, cache_->GetEntryCount());
  EXPECT_THAT(DoomAllEntries(), IsOk());

  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry("Something new", &entry), IsOk());
  entry->Close();

  EXPECT_EQ(1, cache_->GetEntryCount());
}
