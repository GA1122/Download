  void FindMainResponseExclusions(bool drop_from_working_set) {
    MakeCacheAndGroup(kManifestUrl, 1, 1, true);
    cache_->AddEntry(
        kEntryUrl,
        AppCacheEntry(AppCacheEntry::EXPLICIT | AppCacheEntry::FOREIGN, 1));
    cache_->AddEntry(kEntryUrl2, AppCacheEntry(AppCacheEntry::FALLBACK, 2));
    cache_->fallback_namespaces_.push_back(AppCacheNamespace(
        APPCACHE_FALLBACK_NAMESPACE, kFallbackNamespace, kEntryUrl2, false));
    cache_->online_whitelist_namespaces_.push_back(AppCacheNamespace(
        APPCACHE_NETWORK_NAMESPACE, kOnlineNamespace, GURL(), false));
    cache_->online_whitelist_namespaces_.push_back(
        AppCacheNamespace(APPCACHE_NETWORK_NAMESPACE,
                          kOnlineNamespaceWithinFallback, GURL(), false));

    AppCacheDatabase::EntryRecord entry_record;
    entry_record.cache_id = 1;
    entry_record.url = kEntryUrl;
    entry_record.flags = AppCacheEntry::EXPLICIT | AppCacheEntry::FOREIGN;
    entry_record.response_id = 1;
    EXPECT_TRUE(database()->InsertEntry(&entry_record));
    AppCacheDatabase::OnlineWhiteListRecord whitelist_record;
    whitelist_record.cache_id = 1;
    whitelist_record.namespace_url = kOnlineNamespace;
    EXPECT_TRUE(database()->InsertOnlineWhiteList(&whitelist_record));
    AppCacheDatabase::NamespaceRecord fallback_namespace_record;
    fallback_namespace_record.cache_id = 1;
    fallback_namespace_record.namespace_.target_url = kEntryUrl2;
    fallback_namespace_record.namespace_.namespace_url = kFallbackNamespace;
    fallback_namespace_record.origin = url::Origin::Create(kManifestUrl);
    EXPECT_TRUE(database()->InsertNamespace(&fallback_namespace_record));
    whitelist_record.cache_id = 1;
    whitelist_record.namespace_url = kOnlineNamespaceWithinFallback;
    EXPECT_TRUE(database()->InsertOnlineWhiteList(&whitelist_record));
    if (drop_from_working_set) {
      cache_ = nullptr;
      group_ = nullptr;
    }

    PushNextTask(
        base::BindOnce(&AppCacheStorageImplTest::Verify_ExclusionNotFound,
                       base::Unretained(this), kEntryUrl, 1));
    storage()->FindResponseForMainRequest(kEntryUrl, GURL(), delegate());
  }