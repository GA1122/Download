  void MarkEntryAsForeignWithLoadInProgress() {
    PushNextTask(base::BindOnce(
        &AppCacheStorageImplTest::Verify_MarkEntryAsForeignWithLoadInProgress,
        base::Unretained(this)));

    MakeCacheAndGroup(kManifestUrl, 1, 1, true);
    cache_->AddEntry(kEntryUrl, AppCacheEntry(AppCacheEntry::EXPLICIT));
    AppCacheDatabase::EntryRecord entry_record;
    entry_record.cache_id = 1;
    entry_record.url = kEntryUrl;
    entry_record.flags = AppCacheEntry::EXPLICIT;
    entry_record.response_id = 0;
    EXPECT_TRUE(database()->InsertEntry(&entry_record));
    EXPECT_FALSE(cache_->GetEntry(kEntryUrl)->IsForeign());
    EXPECT_TRUE(cache_->HasOneRef());
    cache_ = nullptr;
    group_ = nullptr;

    storage()->LoadCache(1, delegate());
    storage()->MarkEntryAsForeign(kEntryUrl, 1);
  }
