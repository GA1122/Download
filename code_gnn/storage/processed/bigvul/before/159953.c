void DiskCacheBackendTest::BackendBasics() {
  InitCache();
  disk_cache::Entry *entry1 = NULL, *entry2 = NULL;
  EXPECT_NE(net::OK, OpenEntry("the first key", &entry1));
  ASSERT_THAT(CreateEntry("the first key", &entry1), IsOk());
  ASSERT_TRUE(NULL != entry1);
  entry1->Close();
  entry1 = NULL;

  ASSERT_THAT(OpenEntry("the first key", &entry1), IsOk());
  ASSERT_TRUE(NULL != entry1);
  entry1->Close();
  entry1 = NULL;

  EXPECT_NE(net::OK, CreateEntry("the first key", &entry1));
  ASSERT_THAT(OpenEntry("the first key", &entry1), IsOk());
  EXPECT_NE(net::OK, OpenEntry("some other key", &entry2));
  ASSERT_THAT(CreateEntry("some other key", &entry2), IsOk());
  ASSERT_TRUE(NULL != entry1);
  ASSERT_TRUE(NULL != entry2);
  EXPECT_EQ(2, cache_->GetEntryCount());

  disk_cache::Entry* entry3 = NULL;
  ASSERT_THAT(OpenEntry("some other key", &entry3), IsOk());
  ASSERT_TRUE(NULL != entry3);
  EXPECT_TRUE(entry2 == entry3);

  EXPECT_THAT(DoomEntry("some other key"), IsOk());
  EXPECT_EQ(1, cache_->GetEntryCount());
  entry1->Close();
  entry2->Close();
  entry3->Close();

  EXPECT_THAT(DoomEntry("the first key"), IsOk());
  EXPECT_EQ(0, cache_->GetEntryCount());

  ASSERT_THAT(CreateEntry("the first key", &entry1), IsOk());
  ASSERT_THAT(CreateEntry("some other key", &entry2), IsOk());
  entry1->Doom();
  entry1->Close();
  EXPECT_THAT(DoomEntry("some other key"), IsOk());
  EXPECT_EQ(0, cache_->GetEntryCount());
  entry2->Close();
}
