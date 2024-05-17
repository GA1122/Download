void DiskCacheBackendTest::BackendTrimInvalidEntry2() {
  SetMask(0xf);   

  const int kSize = 0x3000;   
  SetMaxSize(kSize * 40);
  InitCache();

  scoped_refptr<net::IOBuffer> buffer(new net::IOBuffer(kSize));
  memset(buffer->data(), 0, kSize);
  disk_cache::Entry* entry;

  for (int i = 0; i < 32; i++) {
    std::string key(base::StringPrintf("some key %d", i));
    ASSERT_THAT(CreateEntry(key, &entry), IsOk());
    EXPECT_EQ(kSize, WriteData(entry, 0, 0, buffer.get(), kSize, false));
    entry->Close();
    ASSERT_THAT(OpenEntry(key, &entry), IsOk());
  }

  SimulateCrash();

  ASSERT_THAT(CreateEntry("Something else", &entry), IsOk());
  EXPECT_EQ(kSize, WriteData(entry, 0, 0, buffer.get(), kSize, false));

  FlushQueueForTest();
  EXPECT_EQ(33, cache_->GetEntryCount());
  SetMaxSize(kSize);

  if (new_eviction_) {
    EXPECT_THAT(DoomAllEntries(), IsOk());
  }

  entry->Close();   
  FlushQueueForTest();

  base::RunLoop().RunUntilIdle();
  FlushQueueForTest();
  ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN();
  EXPECT_GE(30, cache_->GetEntryCount());
  ANNOTATE_IGNORE_READS_AND_WRITES_END();

  size_ = 0;
}
