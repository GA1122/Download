  void Verify_FindMainResponseWithMultipleHits2() {
    EXPECT_EQ(kEntryUrl, delegate()->found_url_);
    EXPECT_EQ(kManifestUrl, delegate()->found_manifest_url_);
    EXPECT_EQ(1, delegate()->found_cache_id_);
    EXPECT_EQ(1, delegate()->found_group_id_);
    EXPECT_EQ(1, delegate()->found_entry_.response_id());
    EXPECT_TRUE(delegate()->found_entry_.IsExplicit());
    EXPECT_FALSE(delegate()->found_fallback_entry_.has_response_id());

    delegate_.reset(new MockStorageDelegate(this));
    PushNextTask(base::BindOnce(
        &AppCacheStorageImplTest::Verify_FindMainResponseWithMultipleHits3,
        base::Unretained(this)));
    storage()->FindResponseForMainRequest(kEntryUrl, kManifestUrl2, delegate());
    EXPECT_NE(kEntryUrl, delegate()->found_url_);
  }
