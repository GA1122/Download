  void Verify_CreateGroup() {
    EXPECT_EQ(kManifestUrl, delegate()->loaded_manifest_url_);
    EXPECT_TRUE(delegate()->loaded_group_.get());
    EXPECT_TRUE(delegate()->loaded_group_->HasOneRef());
    EXPECT_FALSE(delegate()->loaded_group_->newest_complete_cache());

    AppCacheDatabase::GroupRecord record;
    EXPECT_FALSE(
        database()->FindGroup(delegate()->loaded_group_->group_id(), &record));

    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_accessed_count_);
    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_modified_count_);

    TestFinished();
  }
