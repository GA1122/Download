  void Verify_LoadGroup_Far_Hit() {
    EXPECT_TRUE(delegate()->loaded_group_.get());
    EXPECT_EQ(kManifestUrl, delegate()->loaded_manifest_url_);
    EXPECT_TRUE(delegate()->loaded_group_->newest_complete_cache());
    delegate()->loaded_groups_newest_cache_ = nullptr;
    EXPECT_TRUE(delegate()->loaded_group_->HasOneRef());
    EXPECT_EQ(2, mock_quota_manager_proxy_->notify_storage_accessed_count_);
    EXPECT_EQ(0, mock_quota_manager_proxy_->notify_storage_modified_count_);
    TestFinished();
  }
