void DiskCacheBackendTest::BackendInvalidEntryEnumeration() {
  InitCache();

  std::string key("Some key");
  disk_cache::Entry *entry, *entry1, *entry2;
  ASSERT_THAT(CreateEntry(key, &entry1), IsOk());

  const int kSize = 50;
  scoped_refptr<net::IOBuffer> buffer1(new net::IOBuffer(kSize));
  memset(buffer1->data(), 0, kSize);
  base::strlcpy(buffer1->data(), "And the data to save", kSize);
  EXPECT_EQ(kSize, WriteData(entry1, 0, 0, buffer1.get(), kSize, false));
  entry1->Close();
  ASSERT_THAT(OpenEntry(key, &entry1), IsOk());
  EXPECT_EQ(kSize, ReadData(entry1, 0, 0, buffer1.get(), kSize));

  std::string key2("Another key");
  ASSERT_THAT(CreateEntry(key2, &entry2), IsOk());
  entry2->Close();
  ASSERT_EQ(2, cache_->GetEntryCount());

  SimulateCrash();

  std::unique_ptr<TestIterator> iter = CreateIterator();
  int count = 0;
  while (iter->OpenNextEntry(&entry) == net::OK) {
    ASSERT_TRUE(NULL != entry);
    EXPECT_EQ(key2, entry->GetKey());
    entry->Close();
    count++;
  };
  EXPECT_EQ(1, count);
  EXPECT_EQ(1, cache_->GetEntryCount());
}
