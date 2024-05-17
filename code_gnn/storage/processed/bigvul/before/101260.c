  void ExpectUnsyncedUndeletion() {
    EXPECT_EQ(metahandle_, GetMetahandleOfTag());
    EXPECT_FALSE(Get(metahandle_, IS_DEL));
    EXPECT_TRUE(Get(metahandle_, SERVER_IS_DEL));
    EXPECT_EQ(0, Get(metahandle_, BASE_VERSION));
    EXPECT_TRUE(Get(metahandle_, IS_UNSYNCED));
    EXPECT_FALSE(Get(metahandle_, IS_UNAPPLIED_UPDATE));
    EXPECT_TRUE(Get(metahandle_, ID).ServerKnows());
  }
