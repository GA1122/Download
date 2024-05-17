  int64 GetMetahandleOfTag() {
    ReadTransaction trans(FROM_HERE, directory());
    Entry entry(&trans, GET_BY_CLIENT_TAG, client_tag_);
    EXPECT_TRUE(entry.good());
    if (!entry.good()) {
      return syncable::kInvalidMetaHandle;
    }
    return entry.Get(META_HANDLE);
  }
