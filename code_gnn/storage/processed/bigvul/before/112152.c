  bool ResetUnsyncedEntry(syncable::ModelType type,
                          const std::string& client_tag) {
    UserShare* share = sync_manager_.GetUserShare();
    syncable::WriteTransaction trans(
        FROM_HERE, syncable::UNITTEST, share->directory.get());
    const std::string hash = BaseNode::GenerateSyncableHash(type, client_tag);
    syncable::MutableEntry entry(&trans, syncable::GET_BY_CLIENT_TAG,
                                 hash);
    EXPECT_TRUE(entry.good());
    if (!entry.Get(IS_UNSYNCED))
      return false;
    entry.Put(IS_UNSYNCED, false);
    return true;
  }
