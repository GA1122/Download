  void LoadCache_NearHit() {

    int64_t cache_id = storage()->NewCacheId();
    scoped_refptr<AppCache> cache(new AppCache(storage(), cache_id));

    storage()->LoadCache(cache_id, delegate());
    EXPECT_EQ(cache_id, delegate()->loaded_cache_id_);
    EXPECT_EQ(cache.get(), delegate()->loaded_cache_.get());
    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_accessed_count_);
    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_modified_count_);
    TestFinished();
  }
