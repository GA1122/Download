void DiskCacheBackendTest::BackendInvalidEntry10(bool eviction) {
  const int kSize = 0x3000;   
  SetMaxSize(kSize * 10);
  SetNewEviction();
  InitCache();

  std::string first("some key");
  std::string second("something else");
  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry(first, &entry), IsOk());
  entry->Close();
  ASSERT_THAT(OpenEntry(first, &entry), IsOk());
  EXPECT_EQ(0, WriteData(entry, 0, 200, NULL, 0, false));
  entry->Close();
  ASSERT_THAT(CreateEntry(second, &entry), IsOk());

  disk_cache::EntryImpl* entry_impl =
      static_cast<disk_cache::EntryImpl*>(entry);

  entry_impl->entry()->Data()->state = 0xbad;
  entry_impl->entry()->Store();
  entry->Close();
  ASSERT_THAT(CreateEntry("third", &entry), IsOk());
  entry->Close();
  EXPECT_EQ(3, cache_->GetEntryCount());


  if (eviction) {
    TrimForTest(false);
    EXPECT_EQ(3, cache_->GetEntryCount());
    TrimForTest(false);
    EXPECT_EQ(2, cache_->GetEntryCount());
    TrimForTest(false);
    EXPECT_EQ(1, cache_->GetEntryCount());
  } else {
    std::unique_ptr<TestIterator> iter = CreateIterator();
    ASSERT_THAT(iter->OpenNextEntry(&entry), IsOk());
    entry->Close();
    ASSERT_THAT(iter->OpenNextEntry(&entry), IsOk());
    EXPECT_EQ(first, entry->GetKey());
    entry->Close();
    EXPECT_NE(net::OK, iter->OpenNextEntry(&entry));
  }
  DisableIntegrityCheck();
}
