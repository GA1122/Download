  void Delete() {
    WriteTransaction trans(FROM_HERE, UNITTEST, directory());
    MutableEntry entry(&trans, GET_BY_CLIENT_TAG, client_tag_);
    ASSERT_TRUE(entry.good());
    EXPECT_EQ(metahandle_, entry.Get(META_HANDLE));
    entry.Put(IS_DEL, true);
    entry.Put(IS_UNSYNCED, true);
    entry.Put(SYNCING, false);
  }
