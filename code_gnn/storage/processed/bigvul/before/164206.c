  void Verify_BasicFindMainResponse() {
    EXPECT_EQ(kEntryUrl, delegate()->found_url_);
    EXPECT_EQ(kManifestUrl, delegate()->found_manifest_url_);
    EXPECT_EQ(1, delegate()->found_cache_id_);
    EXPECT_EQ(2, delegate()->found_group_id_);
    EXPECT_EQ(1, delegate()->found_entry_.response_id());
    EXPECT_TRUE(delegate()->found_entry_.IsExplicit());
    EXPECT_FALSE(delegate()->found_fallback_entry_.has_response_id());
    TestFinished();
  }
