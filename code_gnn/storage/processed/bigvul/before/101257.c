  void ExpectUnsyncedCreation() {
    EXPECT_EQ(metahandle_, GetMetahandleOfTag());
    EXPECT_FALSE(Get(metahandle_, IS_DEL));
    EXPECT_FALSE(Get(metahandle_, SERVER_IS_DEL));   
    EXPECT_GE(0, Get(metahandle_, BASE_VERSION));
    EXPECT_TRUE(Get(metahandle_, IS_UNSYNCED));
    EXPECT_FALSE(Get(metahandle_, IS_UNAPPLIED_UPDATE));
  }
