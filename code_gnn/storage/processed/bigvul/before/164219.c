  void Verify_FindNoMainResponse() {
    EXPECT_EQ(kEntryUrl, delegate()->found_url_);
    EXPECT_TRUE(delegate()->found_manifest_url_.is_empty());
    EXPECT_EQ(blink::mojom::kAppCacheNoCacheId, delegate()->found_cache_id_);
    EXPECT_EQ(blink::mojom::kAppCacheNoResponseId,
              delegate()->found_entry_.response_id());
    EXPECT_EQ(blink::mojom::kAppCacheNoResponseId,
              delegate()->found_fallback_entry_.response_id());
    EXPECT_TRUE(delegate()->found_namespace_entry_url_.is_empty());
    EXPECT_EQ(0, delegate()->found_entry_.types());
    EXPECT_EQ(0, delegate()->found_fallback_entry_.types());
    TestFinished();
  }
