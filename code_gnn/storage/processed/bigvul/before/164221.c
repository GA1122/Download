  void Verify_LoadCache_Miss() {
    EXPECT_EQ(111, delegate()->loaded_cache_id_);
    EXPECT_FALSE(delegate()->loaded_cache_.get());
    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_accessed_count_);
    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_modified_count_);
    TestFinished();
  }
