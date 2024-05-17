  void WriteTestDataToEntry(WriteTransaction* trans, MutableEntry* entry) {
    EXPECT_FALSE(entry->Get(IS_DIR));
    EXPECT_FALSE(entry->Get(IS_DEL));
    sync_pb::EntitySpecifics specifics;
    specifics.mutable_bookmark()->set_url("http://demo/");
    specifics.mutable_bookmark()->set_favicon("PNG");
    entry->Put(syncable::SPECIFICS, specifics);
    entry->Put(syncable::IS_UNSYNCED, true);
  }
