void DiskCacheBackendTest::BackendEnumerations2() {
  InitCache();
  const std::string first("first");
  const std::string second("second");
  disk_cache::Entry *entry1, *entry2;
  ASSERT_THAT(CreateEntry(first, &entry1), IsOk());
  entry1->Close();
  ASSERT_THAT(CreateEntry(second, &entry2), IsOk());
  entry2->Close();
  FlushQueueForTest();

  AddDelay();
  ASSERT_THAT(OpenEntry(second, &entry1), IsOk());
  std::unique_ptr<TestIterator> iter = CreateIterator();
  ASSERT_THAT(iter->OpenNextEntry(&entry2), IsOk());
  EXPECT_EQ(entry2->GetKey(), second);

  entry1->Close();
  entry2->Close();

  ASSERT_THAT(iter->OpenNextEntry(&entry2), IsOk());
  EXPECT_EQ(entry2->GetKey(), first);
  entry2->Close();
  iter = CreateIterator();

  ASSERT_THAT(OpenEntry(first, &entry1), IsOk());
  EXPECT_EQ(0, WriteData(entry1, 0, 200, NULL, 0, false));
  ASSERT_THAT(iter->OpenNextEntry(&entry2), IsOk());
  if (type_ == net::APP_CACHE) {
    EXPECT_EQ(entry2->GetKey(), second);
  } else {
    EXPECT_EQ(entry2->GetKey(), first);
  }

  entry1->Close();
  entry2->Close();
}
