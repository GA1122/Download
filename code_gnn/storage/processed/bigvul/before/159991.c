void DiskCacheBackendTest::BackendTrimInvalidEntry() {
  const int kSize = 0x3000;   
  SetMaxSize(kSize * 10);
  InitCache();

  std::string first("some key");
  std::string second("something else");
  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry(first, &entry), IsOk());

  scoped_refptr<net::IOBuffer> buffer(new net::IOBuffer(kSize));
  memset(buffer->data(), 0, kSize);
  EXPECT_EQ(kSize, WriteData(entry, 0, 0, buffer.get(), kSize, false));

  SimulateCrash();

  ASSERT_THAT(CreateEntry(second, &entry), IsOk());
  EXPECT_EQ(kSize, WriteData(entry, 0, 0, buffer.get(), kSize, false));

  EXPECT_EQ(2, cache_->GetEntryCount());
  SetMaxSize(kSize);
  entry->Close();   
  FlushQueueForTest();

  base::RunLoop().RunUntilIdle();

  ANNOTATE_IGNORE_READS_BEGIN();
  EXPECT_GE(1, cache_->GetEntryCount());
  ANNOTATE_IGNORE_READS_END();

  EXPECT_NE(net::OK, OpenEntry(first, &entry));
}
