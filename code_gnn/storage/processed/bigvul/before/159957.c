void DiskCacheBackendTest::BackendDisable2() {
  EXPECT_EQ(8, cache_->GetEntryCount());

  disk_cache::Entry* entry;
  std::unique_ptr<TestIterator> iter = CreateIterator();
  int count = 0;
  while (iter->OpenNextEntry(&entry) == net::OK) {
    ASSERT_TRUE(NULL != entry);
    entry->Close();
    count++;
    ASSERT_LT(count, 9);
  };

  FlushQueueForTest();
  EXPECT_EQ(0, cache_->GetEntryCount());
}
