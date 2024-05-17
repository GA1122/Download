  void FindFallbackPatternMatch(bool drop_from_working_set) {
    MakeCacheAndGroup(kManifestUrl, 2, 1, true);
    cache_->AddEntry(kEntryUrl, AppCacheEntry(AppCacheEntry::FALLBACK, 1));
    cache_->fallback_namespaces_.push_back(
        AppCacheNamespace(APPCACHE_FALLBACK_NAMESPACE,
                          kFallbackPatternNamespace, kEntryUrl, true));
    AppCacheDatabase::CacheRecord cache_record;
    std::vector<AppCacheDatabase::EntryRecord> entries;
    std::vector<AppCacheDatabase::NamespaceRecord> intercepts;
    std::vector<AppCacheDatabase::NamespaceRecord> fallbacks;
    std::vector<AppCacheDatabase::OnlineWhiteListRecord> whitelists;
    cache_->ToDatabaseRecords(group_.get(), &cache_record, &entries,
                              &intercepts, &fallbacks, &whitelists);

    for (const auto& entry : entries) {
      if (entry.url != kDefaultEntryUrl)
        EXPECT_TRUE(database()->InsertEntry(&entry));
    }

    EXPECT_TRUE(database()->InsertNamespaceRecords(fallbacks));
    if (drop_from_working_set) {
      EXPECT_TRUE(cache_->HasOneRef());
      cache_ = nullptr;
      EXPECT_TRUE(group_->HasOneRef());
      group_ = nullptr;
    }

    PushNextTask(base::BindOnce(
        &AppCacheStorageImplTest::Verify_FindFallbackPatternMatchNegative,
        base::Unretained(this)));
    storage()->FindResponseForMainRequest(kFallbackPatternTestNegativeUrl,
                                          GURL(), delegate());
    EXPECT_EQ(GURL(), delegate()->found_url_);   
  }
