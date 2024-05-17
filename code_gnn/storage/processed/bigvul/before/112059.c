  void Undelete() {
    WriteTransaction trans(FROM_HERE, UNITTEST, directory());
    MutableEntry entry(&trans, GET_BY_CLIENT_TAG, client_tag_);
    ASSERT_TRUE(entry.good());
    EXPECT_EQ(metahandle_, entry.Get(META_HANDLE));
    EXPECT_TRUE(entry.Get(IS_DEL));
    entry.Put(IS_DEL, false);
    entry.Put(IS_UNSYNCED, true);
    entry.Put(SYNCING, false);
  }