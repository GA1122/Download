void DiskCacheBackendTest::BackendInvalidEntry7() {
  const int kSize = 0x3000;   
  SetMaxSize(kSize * 10);
  InitCache();

  std::string first("some key");
  std::string second("something else");
  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry(first, &entry), IsOk());
  entry->Close();
  ASSERT_THAT(CreateEntry(second, &entry), IsOk());

  disk_cache::EntryImpl* entry_impl =
      static_cast<disk_cache::EntryImpl*>(entry);

  entry_impl->rankings()->Data()->next = 0;
  entry_impl->rankings()->Store();
  entry->Close();
  FlushQueueForTest();
  EXPECT_EQ(2, cache_->GetEntryCount());

  EXPECT_NE(net::OK, OpenEntry(second, &entry));
  EXPECT_EQ(1, cache_->GetEntryCount());

  std::unique_ptr<TestIterator> iter = CreateIterator();
  EXPECT_NE(net::OK, iter->OpenNextEntry(&entry));
  FlushQueueForTest();
  EXPECT_EQ(0, cache_->GetEntryCount());
}
