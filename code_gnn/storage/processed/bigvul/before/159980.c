void DiskCacheBackendTest::BackendKeying() {
  InitCache();
  const char kName1[] = "the first key";
  const char kName2[] = "the first Key";
  disk_cache::Entry *entry1, *entry2;
  ASSERT_THAT(CreateEntry(kName1, &entry1), IsOk());

  ASSERT_THAT(CreateEntry(kName2, &entry2), IsOk());
  EXPECT_TRUE(entry1 != entry2) << "Case sensitive";
  entry2->Close();

  char buffer[30];
  base::strlcpy(buffer, kName1, arraysize(buffer));
  ASSERT_THAT(OpenEntry(buffer, &entry2), IsOk());
  EXPECT_TRUE(entry1 == entry2);
  entry2->Close();

  base::strlcpy(buffer + 1, kName1, arraysize(buffer) - 1);
  ASSERT_THAT(OpenEntry(buffer + 1, &entry2), IsOk());
  EXPECT_TRUE(entry1 == entry2);
  entry2->Close();

  base::strlcpy(buffer + 3, kName1, arraysize(buffer) - 3);
  ASSERT_THAT(OpenEntry(buffer + 3, &entry2), IsOk());
  EXPECT_TRUE(entry1 == entry2);
  entry2->Close();

  char buffer2[20000];
  memset(buffer2, 's', sizeof(buffer2));
  buffer2[1023] = '\0';
  ASSERT_EQ(net::OK, CreateEntry(buffer2, &entry2)) << "key on block file";
  entry2->Close();

  buffer2[1023] = 'g';
  buffer2[19999] = '\0';
  ASSERT_EQ(net::OK, CreateEntry(buffer2, &entry2)) << "key on external file";
  entry2->Close();
  entry1->Close();
}
