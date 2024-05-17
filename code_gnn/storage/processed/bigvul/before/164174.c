  void FindNoMainResponse() {
    PushNextTask(
        base::BindOnce(&AppCacheStorageImplTest::Verify_FindNoMainResponse,
                       base::Unretained(this)));

    storage()->FindResponseForMainRequest(kEntryUrl, GURL(), delegate());
    EXPECT_NE(kEntryUrl, delegate()->found_url_);
  }
