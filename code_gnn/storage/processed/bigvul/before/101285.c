  void VerifyTestBookmarkDataInEntry(Entry* entry) {
    const sync_pb::EntitySpecifics& specifics = entry->Get(syncable::SPECIFICS);
    EXPECT_TRUE(specifics.HasExtension(sync_pb::bookmark));
    EXPECT_EQ("PNG", specifics.GetExtension(sync_pb::bookmark).favicon());
    EXPECT_EQ("http://demo/", specifics.GetExtension(sync_pb::bookmark).url());
  }
