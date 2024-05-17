  void Create() {
    WriteTransaction trans(FROM_HERE, UNITTEST, directory());
    MutableEntry perm_folder(&trans, CREATE, ids_.root(), "clientname");
    ASSERT_TRUE(perm_folder.good());
    perm_folder.Put(UNIQUE_CLIENT_TAG, client_tag_);
    perm_folder.Put(IS_UNSYNCED, true);
    perm_folder.Put(SYNCING, false);
    perm_folder.Put(SPECIFICS, DefaultBookmarkSpecifics());
    EXPECT_FALSE(perm_folder.Get(IS_UNAPPLIED_UPDATE));
    EXPECT_FALSE(perm_folder.Get(ID).ServerKnows());
    metahandle_ = perm_folder.Get(META_HANDLE);
  }
