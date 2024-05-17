  void Verify_MarkEntryAsForeignWithLoadInProgress() {
    EXPECT_EQ(1, delegate()->loaded_cache_id_);
    EXPECT_TRUE(delegate()->loaded_cache_.get());

    EXPECT_TRUE(delegate()->loaded_cache_->GetEntry(kEntryUrl)->IsForeign());
    EXPECT_TRUE(delegate()->loaded_cache_->GetEntry(kEntryUrl)->IsExplicit());

    FlushAllTasks();
    AppCacheDatabase::EntryRecord entry_record;
    EXPECT_TRUE(database()->FindEntry(1, kEntryUrl, &entry_record));
    EXPECT_EQ(AppCacheEntry::EXPLICIT | AppCacheEntry::FOREIGN,
              entry_record.flags);
    TestFinished();
  }
