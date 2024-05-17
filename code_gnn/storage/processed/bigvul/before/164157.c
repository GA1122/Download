  void BasicFindMainResponse(bool drop_from_working_set) {
    PushNextTask(
        base::BindOnce(&AppCacheStorageImplTest::Verify_BasicFindMainResponse,
                       base::Unretained(this)));

    MakeCacheAndGroup(kManifestUrl, 2, 1, true);
    cache_->AddEntry(kEntryUrl, AppCacheEntry(AppCacheEntry::EXPLICIT, 1));
    AppCacheDatabase::EntryRecord entry_record;
    entry_record.cache_id = 1;
    entry_record.url = kEntryUrl;
    entry_record.flags = AppCacheEntry::EXPLICIT;
    entry_record.response_id = 1;
    EXPECT_TRUE(database()->InsertEntry(&entry_record));

    if (drop_from_working_set) {
      EXPECT_TRUE(cache_->HasOneRef());
      cache_ = nullptr;
      EXPECT_TRUE(group_->HasOneRef());
      group_ = nullptr;
    }

    storage()->FindResponseForMainRequest(kEntryUrl, GURL(), delegate());
    EXPECT_NE(kEntryUrl, delegate()->found_url_);
  }
