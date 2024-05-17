void DiskCacheBackendTest::BackendShutdownWithPendingDoom() {
  net::TestCompletionCallback cb;
  {
    ASSERT_TRUE(CleanupCacheDir());

    disk_cache::BackendFlags flags = disk_cache::kNoRandom;
    CreateBackend(flags);

    disk_cache::Entry* entry;
    int rv = cache_->CreateEntry("some key", &entry, cb.callback());
    ASSERT_THAT(cb.GetResult(rv), IsOk());
    entry->Close();
    entry = nullptr;

    rv = cache_->DoomEntry("some key", cb.callback());
    ASSERT_THAT(rv, IsError(net::ERR_IO_PENDING));

    cache_.reset();
    EXPECT_FALSE(cb.have_result());
  }

  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(cb.have_result());
}
