  void Verify_FindMainResponseWithMultipleHits4() {
    EXPECT_EQ(kFallbackTestUrl, delegate()->found_url_);
    EXPECT_EQ(kManifestUrl3, delegate()->found_manifest_url_);
    EXPECT_EQ(3, delegate()->found_cache_id_);
    EXPECT_EQ(3, delegate()->found_group_id_);
    EXPECT_FALSE(delegate()->found_entry_.has_response_id());
    EXPECT_EQ(3 + kFallbackEntryIdOffset,
              delegate()->found_fallback_entry_.response_id());
    EXPECT_TRUE(delegate()->found_fallback_entry_.IsFallback());
    EXPECT_EQ(kEntryUrl2, delegate()->found_namespace_entry_url_);

    delegate_.reset(new MockStorageDelegate(this));
    PushNextTask(base::BindOnce(
        &AppCacheStorageImplTest::Verify_FindMainResponseWithMultipleHits5,
        base::Unretained(this)));
    storage()->FindResponseForMainRequest(kFallbackTestUrl, kManifestUrl2,
                                          delegate());
    EXPECT_NE(kFallbackTestUrl, delegate()->found_url_);
  }
