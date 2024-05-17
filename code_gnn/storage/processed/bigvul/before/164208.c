  void Verify_ExclusionNotFound(GURL expected_url, int phase) {
    EXPECT_EQ(expected_url, delegate()->found_url_);
    EXPECT_TRUE(delegate()->found_manifest_url_.is_empty());
    EXPECT_EQ(blink::mojom::kAppCacheNoCacheId, delegate()->found_cache_id_);
    EXPECT_EQ(0, delegate()->found_group_id_);
    EXPECT_EQ(blink::mojom::kAppCacheNoResponseId,
              delegate()->found_entry_.response_id());
    EXPECT_EQ(blink::mojom::kAppCacheNoResponseId,
              delegate()->found_fallback_entry_.response_id());
    EXPECT_TRUE(delegate()->found_namespace_entry_url_.is_empty());
    EXPECT_EQ(0, delegate()->found_entry_.types());
    EXPECT_EQ(0, delegate()->found_fallback_entry_.types());

    if (phase == 1) {
      PushNextTask(
          base::BindOnce(&AppCacheStorageImplTest::Verify_ExclusionNotFound,
                         base::Unretained(this), kOnlineNamespace, 2));
      storage()->FindResponseForMainRequest(kOnlineNamespace, GURL(),
                                            delegate());
      return;
    }
    if (phase == 2) {
      PushNextTask(base::BindOnce(
          &AppCacheStorageImplTest::Verify_ExclusionNotFound,
          base::Unretained(this), kOnlineNamespaceWithinFallback, 3));
      storage()->FindResponseForMainRequest(kOnlineNamespaceWithinFallback,
                                            GURL(), delegate());
      return;
    }

    TestFinished();
  }
