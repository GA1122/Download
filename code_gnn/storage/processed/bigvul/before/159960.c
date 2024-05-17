void DiskCacheBackendTest::BackendDoomAll() {
  InitCache();

  disk_cache::Entry *entry1, *entry2;
  ASSERT_THAT(CreateEntry("first", &entry1), IsOk());
  ASSERT_THAT(CreateEntry("second", &entry2), IsOk());
  entry1->Close();
  entry2->Close();

  ASSERT_THAT(CreateEntry("third", &entry1), IsOk());
  ASSERT_THAT(CreateEntry("fourth", &entry2), IsOk());

  ASSERT_EQ(4, cache_->GetEntryCount());
  EXPECT_THAT(DoomAllEntries(), IsOk());
  ASSERT_EQ(0, cache_->GetEntryCount());

  base::RunLoop().RunUntilIdle();

  disk_cache::Entry *entry3, *entry4;
  EXPECT_NE(net::OK, OpenEntry("third", &entry3));
  ASSERT_THAT(CreateEntry("third", &entry3), IsOk());
  ASSERT_THAT(CreateEntry("fourth", &entry4), IsOk());

  EXPECT_THAT(DoomAllEntries(), IsOk());
  ASSERT_EQ(0, cache_->GetEntryCount());

  entry1->Close();
  entry2->Close();
  entry3->Doom();   
  entry3->Close();
  entry4->Close();

  ASSERT_THAT(CreateEntry("third", &entry1), IsOk());
  ASSERT_THAT(CreateEntry("fourth", &entry2), IsOk());
  entry1->Close();
  entry2->Close();

  ASSERT_EQ(2, cache_->GetEntryCount());
  EXPECT_THAT(DoomAllEntries(), IsOk());
  ASSERT_EQ(0, cache_->GetEntryCount());

  EXPECT_THAT(DoomAllEntries(), IsOk());
}
