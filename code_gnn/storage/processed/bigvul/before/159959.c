void DiskCacheBackendTest::BackendDisabledAPI() {
  cache_impl_->SetUnitTestMode();   

  disk_cache::Entry *entry1, *entry2;
  std::unique_ptr<TestIterator> iter = CreateIterator();
  EXPECT_EQ(2, cache_->GetEntryCount());
  ASSERT_THAT(iter->OpenNextEntry(&entry1), IsOk());
  entry1->Close();
  EXPECT_NE(net::OK, iter->OpenNextEntry(&entry2));
  FlushQueueForTest();

  EXPECT_EQ(net::DISK_CACHE, cache_->GetCacheType());
  EXPECT_EQ(0, cache_->GetEntryCount());
  EXPECT_NE(net::OK, OpenEntry("First", &entry2));
  EXPECT_NE(net::OK, CreateEntry("Something new", &entry2));
  EXPECT_NE(net::OK, DoomEntry("First"));
  EXPECT_NE(net::OK, DoomAllEntries());
  EXPECT_NE(net::OK, DoomEntriesBetween(Time(), Time::Now()));
  EXPECT_NE(net::OK, DoomEntriesSince(Time()));
  iter = CreateIterator();
  EXPECT_NE(net::OK, iter->OpenNextEntry(&entry2));

  base::StringPairs stats;
  cache_->GetStats(&stats);
  EXPECT_TRUE(stats.empty());
  cache_->OnExternalCacheHit("First");
}
