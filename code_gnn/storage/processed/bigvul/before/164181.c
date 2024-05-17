  void MakeMultipleHitCacheAndGroup(const GURL& manifest_url, int id) {
    MakeCacheAndGroup(manifest_url, id, id, true);
    AppCacheDatabase::EntryRecord entry_record;

    entry_record.cache_id = id;
    entry_record.url = kEntryUrl;
    entry_record.flags = AppCacheEntry::EXPLICIT;
    entry_record.response_id = id;
    EXPECT_TRUE(database()->InsertEntry(&entry_record));
    cache_->AddEntry(entry_record.url, AppCacheEntry(entry_record.flags,
                                                     entry_record.response_id));

    entry_record.cache_id = id;
    entry_record.url = manifest_url;
    entry_record.flags = AppCacheEntry::MANIFEST;
    entry_record.response_id = id + kManifestEntryIdOffset;
    EXPECT_TRUE(database()->InsertEntry(&entry_record));
    cache_->AddEntry(entry_record.url, AppCacheEntry(entry_record.flags,
                                                     entry_record.response_id));

    entry_record.cache_id = id;
    entry_record.url = kEntryUrl2;
    entry_record.flags = AppCacheEntry::FALLBACK;
    entry_record.response_id = id + kFallbackEntryIdOffset;
    EXPECT_TRUE(database()->InsertEntry(&entry_record));
    cache_->AddEntry(entry_record.url, AppCacheEntry(entry_record.flags,
                                                     entry_record.response_id));
    AppCacheDatabase::NamespaceRecord fallback_namespace_record;
    fallback_namespace_record.cache_id = id;
    fallback_namespace_record.namespace_.target_url = entry_record.url;
    fallback_namespace_record.namespace_.namespace_url = kFallbackNamespace;
    fallback_namespace_record.origin = url::Origin::Create(manifest_url);
    EXPECT_TRUE(database()->InsertNamespace(&fallback_namespace_record));
    cache_->fallback_namespaces_.push_back(AppCacheNamespace(
        APPCACHE_FALLBACK_NAMESPACE, kFallbackNamespace, kEntryUrl2, false));
  }
