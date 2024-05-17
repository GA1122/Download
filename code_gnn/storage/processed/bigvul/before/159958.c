void DiskCacheBackendTest::BackendDisable4() {
  disk_cache::Entry *entry1, *entry2, *entry3, *entry4;
  std::unique_ptr<TestIterator> iter = CreateIterator();
  ASSERT_THAT(iter->OpenNextEntry(&entry1), IsOk());

  char key2[2000];
  char key3[20000];
  CacheTestFillBuffer(key2, sizeof(key2), true);
  CacheTestFillBuffer(key3, sizeof(key3), true);
  key2[sizeof(key2) - 1] = '\0';
  key3[sizeof(key3) - 1] = '\0';
  ASSERT_THAT(CreateEntry(key2, &entry2), IsOk());
  ASSERT_THAT(CreateEntry(key3, &entry3), IsOk());

  const int kBufSize = 20000;
  scoped_refptr<net::IOBuffer> buf(new net::IOBuffer(kBufSize));
  memset(buf->data(), 0, kBufSize);
  EXPECT_EQ(100, WriteData(entry2, 0, 0, buf.get(), 100, false));
  EXPECT_EQ(kBufSize, WriteData(entry3, 0, 0, buf.get(), kBufSize, false));

  EXPECT_NE(net::OK, iter->OpenNextEntry(&entry4));
  EXPECT_EQ(0, cache_->GetEntryCount());

  EXPECT_NE(net::OK, CreateEntry("cache is disabled", &entry4));

  EXPECT_EQ(100, ReadData(entry2, 0, 0, buf.get(), 100));
  EXPECT_EQ(100, WriteData(entry2, 0, 0, buf.get(), 100, false));
  EXPECT_EQ(100, WriteData(entry2, 1, 0, buf.get(), 100, false));

  EXPECT_EQ(kBufSize, ReadData(entry3, 0, 0, buf.get(), kBufSize));
  EXPECT_EQ(kBufSize, WriteData(entry3, 0, 0, buf.get(), kBufSize, false));
  EXPECT_EQ(kBufSize, WriteData(entry3, 1, 0, buf.get(), kBufSize, false));

  std::string key = entry2->GetKey();
  EXPECT_EQ(sizeof(key2) - 1, key.size());
  key = entry3->GetKey();
  EXPECT_EQ(sizeof(key3) - 1, key.size());

  entry1->Close();
  entry2->Close();
  entry3->Close();
  FlushQueueForTest();   
  FlushQueueForTest();   

  EXPECT_EQ(0, cache_->GetEntryCount());
}
