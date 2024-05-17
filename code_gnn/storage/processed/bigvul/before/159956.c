void DiskCacheBackendTest::BackendDisable() {
  disk_cache::Entry *entry1, *entry2;
  std::unique_ptr<TestIterator> iter = CreateIterator();
  ASSERT_THAT(iter->OpenNextEntry(&entry1), IsOk());

  EXPECT_NE(net::OK, iter->OpenNextEntry(&entry2));
  EXPECT_EQ(0, cache_->GetEntryCount());
  EXPECT_NE(net::OK, CreateEntry("Something new", &entry2));

  entry1->Close();
  FlushQueueForTest();   
  FlushQueueForTest();   

  EXPECT_EQ(0, cache_->GetEntryCount());
}
