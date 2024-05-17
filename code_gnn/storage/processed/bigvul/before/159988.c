void DiskCacheBackendTest::BackendShutdownWithPendingFileIO(bool fast) {
  ASSERT_TRUE(CleanupCacheDir());
  uint32_t flags = disk_cache::kNoBuffering;
  if (!fast)
    flags |= disk_cache::kNoRandom;

  if (!simple_cache_mode_)
    UseCurrentThread();
  CreateBackend(flags);

  net::TestCompletionCallback cb;
  int rv = GeneratePendingIO(&cb);

  cache_.reset();

  if (rv == net::ERR_IO_PENDING) {
    if (fast || simple_cache_mode_)
      EXPECT_FALSE(cb.have_result());
    else
      EXPECT_TRUE(cb.have_result());
  }

  base::RunLoop().RunUntilIdle();

#if !defined(OS_IOS)
  rv = cb.GetResult(rv);
#endif
}
