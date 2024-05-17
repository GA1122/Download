void DiskCacheBackendTest::BackendInvalidEntry2() {
  ASSERT_TRUE(CopyTestCache("bad_entry"));
  DisableFirstCleanup();
  InitCache();

  disk_cache::Entry *entry1, *entry2;
  ASSERT_THAT(OpenEntry("the first key", &entry1), IsOk());
  EXPECT_NE(net::OK, OpenEntry("some other key", &entry2));
  entry1->Close();

  DisableIntegrityCheck();
}
