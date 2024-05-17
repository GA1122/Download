  void MarkEntryAsForeign() {
    MakeCacheAndGroup(kManifestUrl, 1, 1, true);
    cache_->AddEntry(kEntryUrl, AppCacheEntry(AppCacheEntry::EXPLICIT));
    AppCacheDatabase::EntryRecord entry_record;
    entry_record.cache_id = 1;
    entry_record.url = kEntryUrl;
    entry_record.flags = AppCacheEntry::EXPLICIT;
    entry_record.response_id = 0;
    EXPECT_TRUE(database()->InsertEntry(&entry_record));
    EXPECT_FALSE(cache_->GetEntry(kEntryUrl)->IsForeign());

    storage()->MarkEntryAsForeign(kEntryUrl, 1);

    EXPECT_TRUE(cache_->GetEntry(kEntryUrl)->IsForeign());
    EXPECT_TRUE(cache_->GetEntry(kEntryUrl)->IsExplicit());

    FlushAllTasks();
    AppCacheDatabase::EntryRecord entry_record2;
    EXPECT_TRUE(database()->FindEntry(1, kEntryUrl, &entry_record2));
    EXPECT_EQ(AppCacheEntry::EXPLICIT | AppCacheEntry::FOREIGN,
              entry_record2.flags);
    TestFinished();
  }
