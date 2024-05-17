void DiskCacheBackendTest::BackendCalculateSizeOfAllEntries() {
  InitCache();

  EXPECT_EQ(0, CalculateSizeOfAllEntries());

  std::set<std::string> key_pool;
  CreateSetOfRandomEntries(&key_pool);

  int count = 0;
  int total_size = 0;
  for (std::string key : key_pool) {
    std::string data(count, ' ');
    scoped_refptr<net::StringIOBuffer> buffer = new net::StringIOBuffer(data);

    disk_cache::Entry* entry;
    ASSERT_THAT(OpenEntry(key, &entry), IsOk());
    ASSERT_EQ(count, WriteData(entry, count % 2, 0, buffer.get(), count, true));
    entry->Close();

    total_size += GetRoundedSize(count + GetEntryMetadataSize(key));
    ++count;
  }

  int result = CalculateSizeOfAllEntries();
  EXPECT_EQ(total_size, result);

  {
    const int last_entry_size = 47;
    std::string data(last_entry_size, ' ');
    scoped_refptr<net::StringIOBuffer> buffer = new net::StringIOBuffer(data);

    disk_cache::Entry* entry;
    std::string key = GenerateKey(true);
    ASSERT_THAT(CreateEntry(key, &entry), IsOk());
    ASSERT_EQ(last_entry_size,
              WriteData(entry, 0, 0, buffer.get(), last_entry_size, true));
    entry->Close();

    int new_result = CalculateSizeOfAllEntries();
    EXPECT_EQ(
        result + GetRoundedSize(last_entry_size + GetEntryMetadataSize(key)),
        new_result);

    DoomEntry(key);
    new_result = CalculateSizeOfAllEntries();
    EXPECT_EQ(result, new_result);
  }

  ASSERT_THAT(DoomAllEntries(), IsOk());
  EXPECT_EQ(0, CalculateSizeOfAllEntries());
}
