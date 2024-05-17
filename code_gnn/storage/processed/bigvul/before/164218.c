  void Verify_FindMainResponseWithMultipleHits5() {
    EXPECT_EQ(kFallbackTestUrl, delegate()->found_url_);
    EXPECT_EQ(kManifestUrl2, delegate()->found_manifest_url_);
    EXPECT_EQ(2, delegate()->found_cache_id_);
    EXPECT_EQ(2, delegate()->found_group_id_);
    EXPECT_FALSE(delegate()->found_entry_.has_response_id());
    EXPECT_EQ(2 + kFallbackEntryIdOffset,
              delegate()->found_fallback_entry_.response_id());
    EXPECT_TRUE(delegate()->found_fallback_entry_.IsFallback());
    EXPECT_EQ(kEntryUrl2, delegate()->found_namespace_entry_url_);

    TestFinished();
  }
