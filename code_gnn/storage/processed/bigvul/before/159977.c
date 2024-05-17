void DiskCacheBackendTest::BackendInvalidEntryRead() {
  InitCache();

  std::string key("Some key");
  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry(key, &entry), IsOk());

  const int kSize = 50;
  scoped_refptr<net::IOBuffer> buffer(new net::IOBuffer(kSize));
  memset(buffer->data(), 0, kSize);
  base::strlcpy(buffer->data(), "And the data to save", kSize);
  EXPECT_EQ(kSize, WriteData(entry, 0, 0, buffer.get(), kSize, false));
  entry->Close();
  ASSERT_THAT(OpenEntry(key, &entry), IsOk());
  EXPECT_EQ(kSize, ReadData(entry, 0, 0, buffer.get(), kSize));

  SimulateCrash();

  if (type_ == net::APP_CACHE) {
    ASSERT_THAT(OpenEntry(key, &entry), IsOk());
    EXPECT_EQ(1, cache_->GetEntryCount());
    entry->Close();
  } else {
    EXPECT_NE(net::OK, OpenEntry(key, &entry));
    EXPECT_EQ(0, cache_->GetEntryCount());
  }
}
