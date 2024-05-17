void DiskCacheBackendTest::BackendShutdownWithPendingCreate(bool fast) {
  net::TestCompletionCallback cb;

  {
    ASSERT_TRUE(CleanupCacheDir());

    disk_cache::BackendFlags flags =
        fast ? disk_cache::kNone : disk_cache::kNoRandom;
    CreateBackend(flags);

    disk_cache::Entry* entry;
    int rv = cache_->CreateEntry("some key", &entry, cb.callback());
    ASSERT_THAT(rv, IsError(net::ERR_IO_PENDING));

    cache_.reset();
    EXPECT_FALSE(cb.have_result());
  }

  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(cb.have_result());
}
