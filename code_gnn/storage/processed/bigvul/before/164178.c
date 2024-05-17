  void LoadCache_Miss() {
    PushNextTask(base::BindOnce(&AppCacheStorageImplTest::Verify_LoadCache_Miss,
                                base::Unretained(this)));

    storage()->LoadCache(111, delegate());
    EXPECT_NE(111, delegate()->loaded_cache_id_);
  }
