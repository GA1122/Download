void DiskCacheBackendTest::BackendValidEntry() {
  InitCache();

  std::string key("Some key");
  disk_cache::Entry* entry;
  ASSERT_THAT(CreateEntry(key, &entry), IsOk());

  const int kSize = 50;
  scoped_refptr<net::IOBuffer> buffer1(new net::IOBuffer(kSize));
  memset(buffer1->data(), 0, kSize);
  base::strlcpy(buffer1->data(), "And the data to save", kSize);
  EXPECT_EQ(kSize, WriteData(entry, 0, 0, buffer1.get(), kSize, false));
  entry->Close();
  SimulateCrash();

  ASSERT_THAT(OpenEntry(key, &entry), IsOk());

  scoped_refptr<net::IOBuffer> buffer2(new net::IOBuffer(kSize));
  memset(buffer2->data(), 0, kSize);
  EXPECT_EQ(kSize, ReadData(entry, 0, 0, buffer2.get(), kSize));
  entry->Close();
  EXPECT_STREQ(buffer1->data(), buffer2->data());
}
