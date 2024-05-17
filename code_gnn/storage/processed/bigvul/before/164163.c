  void CreateGroupInPopulatedOrigin() {
    PushNextTask(base::BindOnce(&AppCacheStorageImplTest::Verify_CreateGroup,
                                base::Unretained(this)));

    storage()->usage_map_[kOrigin] = kDefaultEntrySize;

    storage()->LoadOrCreateGroup(kManifestUrl, delegate());
    EXPECT_FALSE(delegate()->loaded_group_.get());
  }
