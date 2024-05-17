  void Verify_FindInterceptPatternMatchPositive() {
    EXPECT_EQ(kInterceptPatternTestPositiveUrl, delegate()->found_url_);
    EXPECT_EQ(kManifestUrl, delegate()->found_manifest_url_);
    EXPECT_EQ(1, delegate()->found_cache_id_);
    EXPECT_EQ(2, delegate()->found_group_id_);
    EXPECT_EQ(1, delegate()->found_entry_.response_id());
    EXPECT_TRUE(delegate()->found_entry_.IsIntercept());
    EXPECT_EQ(kEntryUrl, delegate()->found_namespace_entry_url_);
    EXPECT_FALSE(delegate()->found_fallback_entry_.has_response_id());
    TestFinished();
  }
