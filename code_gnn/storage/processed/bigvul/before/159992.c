void DiskCacheBackendTest::BackendTrimInvalidEntry12() {
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

  entry_impl->entry()->Data()->state = 0xbad;
  entry_impl->entry()->Store();
  entry->Close();
  ASSERT_THAT(CreateEntry("third", &entry), IsOk());
  entry->Close();
  ASSERT_THAT(CreateEntry("fourth", &entry), IsOk());
  TrimForTest(true);
  EXPECT_EQ(1, cache_->GetEntryCount());
  entry->Close();
  DisableIntegrityCheck();
}
