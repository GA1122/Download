  void VerifyGetCacheState(base::PlatformFileError error, int cache_state) {
    ++num_callback_invocations_;

    EXPECT_EQ(expected_error_, error);

    if (error == base::PLATFORM_FILE_OK) {
      EXPECT_EQ(expected_cache_state_, cache_state);
    }
  }
