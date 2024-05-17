void DiskCacheBackendTest::BackendShutdownWithPendingIO(bool fast) {
  net::TestCompletionCallback cb;

  {
    ASSERT_TRUE(CleanupCacheDir());

    uint32_t flags = disk_cache::kNoBuffering;
    if (!fast)
      flags |= disk_cache::kNoRandom;

    CreateBackend(flags);

    disk_cache::Entry* entry;
    int rv = cache_->CreateEntry("some key", &entry, cb.callback());
    ASSERT_THAT(cb.GetResult(rv), IsOk());

    entry->Close();

    cache_.reset();
  }

  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(cb.have_result());
}
