  void FindMainResponseWithMultipleHits() {
    PushNextTask(base::BindOnce(
        &AppCacheStorageImplTest::Verify_FindMainResponseWithMultipleHits,
        base::Unretained(this)));

    MakeMultipleHitCacheAndGroup(kManifestUrl, 1);
    MakeMultipleHitCacheAndGroup(kManifestUrl2, 2);
    MakeMultipleHitCacheAndGroup(kManifestUrl3, 3);

    storage()->FindResponseForMainRequest(kEntryUrl, GURL(), delegate());
    EXPECT_NE(kEntryUrl, delegate()->found_url_);
  }
