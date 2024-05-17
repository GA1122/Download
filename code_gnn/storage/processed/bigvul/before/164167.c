  void FindInterceptPatternMatch(bool drop_from_working_set) {
    MakeCacheAndGroup(kManifestUrl, 2, 1, true);
    cache_->AddEntry(kEntryUrl, AppCacheEntry(AppCacheEntry::INTERCEPT, 1));
    cache_->intercept_namespaces_.push_back(
        AppCacheNamespace(APPCACHE_INTERCEPT_NAMESPACE,
                          kInterceptPatternNamespace, kEntryUrl, true));
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

    EXPECT_TRUE(database()->InsertNamespaceRecords(intercepts));
    if (drop_from_working_set) {
      EXPECT_TRUE(cache_->HasOneRef());
      cache_ = nullptr;
      EXPECT_TRUE(group_->HasOneRef());
      group_ = nullptr;
    }

    PushNextTask(base::BindOnce(
        &AppCacheStorageImplTest::Verify_FindInterceptPatternMatchNegative,
        base::Unretained(this)));
    storage()->FindResponseForMainRequest(kInterceptPatternTestNegativeUrl,
                                          GURL(), delegate());
    EXPECT_EQ(GURL(), delegate()->found_url_);   
  }
