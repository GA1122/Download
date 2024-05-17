void DiskCacheBackendTest::InitSparseCache(base::Time* doomed_start,
                                           base::Time* doomed_end) {
  InitCache();

  const int kSize = 50;
  const int kOffset = 10 + 1024 * 1024;

  disk_cache::Entry* entry0 = NULL;
  disk_cache::Entry* entry1 = NULL;
  disk_cache::Entry* entry2 = NULL;

  scoped_refptr<net::IOBuffer> buffer(new net::IOBuffer(kSize));
  CacheTestFillBuffer(buffer->data(), kSize, false);

  ASSERT_THAT(CreateEntry("zeroth", &entry0), IsOk());
  ASSERT_EQ(kSize, WriteSparseData(entry0, 0, buffer.get(), kSize));
  ASSERT_EQ(kSize,
            WriteSparseData(entry0, kOffset + kSize, buffer.get(), kSize));
  entry0->Close();

  FlushQueueForTest();
  AddDelay();
  if (doomed_start)
    *doomed_start = base::Time::Now();

  ASSERT_THAT(CreateEntry("first", &entry1), IsOk());
  ASSERT_EQ(kSize, WriteSparseData(entry1, 0, buffer.get(), kSize));
  ASSERT_EQ(kSize,
            WriteSparseData(entry1, kOffset + kSize, buffer.get(), kSize));
  entry1->Close();

  ASSERT_THAT(CreateEntry("second", &entry2), IsOk());
  ASSERT_EQ(kSize, WriteSparseData(entry2, 0, buffer.get(), kSize));
  ASSERT_EQ(kSize,
            WriteSparseData(entry2, kOffset + kSize, buffer.get(), kSize));
  entry2->Close();

  FlushQueueForTest();
  AddDelay();
  if (doomed_end)
    *doomed_end = base::Time::Now();

  disk_cache::Entry* entry3 = NULL;
  disk_cache::Entry* entry4 = NULL;
  ASSERT_THAT(CreateEntry("third", &entry3), IsOk());
  ASSERT_EQ(kSize, WriteSparseData(entry3, 0, buffer.get(), kSize));
  ASSERT_THAT(CreateEntry("fourth", &entry4), IsOk());
  ASSERT_EQ(kSize, WriteSparseData(entry4, 0, buffer.get(), kSize));
  ASSERT_EQ(kSize,
            WriteSparseData(entry3, kOffset + kSize, buffer.get(), kSize));
  ASSERT_EQ(kSize,
            WriteSparseData(entry4, kOffset + kSize, buffer.get(), kSize));
  entry3->Close();
  entry4->Close();

  FlushQueueForTest();
  AddDelay();
}
