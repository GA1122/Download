  void Verify_FailStoreGroup() {
    EXPECT_FALSE(delegate()->stored_group_success_);
    EXPECT_TRUE(delegate()->would_exceed_quota_);

    AppCacheDatabase::GroupRecord group_record;
    AppCacheDatabase::CacheRecord cache_record;
    EXPECT_FALSE(database()->FindGroup(group_->group_id(), &group_record));
    EXPECT_FALSE(database()->FindCache(cache_->cache_id(), &cache_record));

    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_accessed_count_);
    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_modified_count_);

    TestFinished();
  }
