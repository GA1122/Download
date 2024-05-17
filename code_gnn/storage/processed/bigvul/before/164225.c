  void Verify_StoreNewGroup() {
    EXPECT_TRUE(delegate()->stored_group_success_);
    EXPECT_EQ(group_.get(), delegate()->stored_group_.get());
    EXPECT_EQ(cache_.get(), group_->newest_complete_cache());
    EXPECT_TRUE(cache_->is_complete());

    AppCacheDatabase::GroupRecord group_record;
    AppCacheDatabase::CacheRecord cache_record;
    EXPECT_TRUE(database()->FindGroup(group_->group_id(), &group_record));
    EXPECT_TRUE(database()->FindCache(cache_->cache_id(), &cache_record));

    EXPECT_EQ(kDefaultEntrySize, storage()->usage_map_[kOrigin]);
    EXPECT_EQ(1, mock_quota_manager_proxy_->notify_storage_modified_count_);
    EXPECT_EQ(kOrigin, mock_quota_manager_proxy_->last_origin_);
    EXPECT_EQ(kDefaultEntrySize, mock_quota_manager_proxy_->last_delta_);

    TestFinished();
  }
