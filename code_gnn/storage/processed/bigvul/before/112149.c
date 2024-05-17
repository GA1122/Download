int64 MakeServerNode(UserShare* share, ModelType model_type,
                     const std::string& client_tag,
                     const std::string& hashed_tag,
                     const sync_pb::EntitySpecifics& specifics) {
  syncable::WriteTransaction trans(
      FROM_HERE, syncable::UNITTEST, share->directory.get());
  syncable::Entry root_entry(&trans, syncable::GET_BY_SERVER_TAG,
                             syncable::ModelTypeToRootTag(model_type));
  EXPECT_TRUE(root_entry.good());
  syncable::Id root_id = root_entry.Get(syncable::ID);
  syncable::Id node_id = syncable::Id::CreateFromServerId(client_tag);
  syncable::MutableEntry entry(&trans, syncable::CREATE_NEW_UPDATE_ITEM,
                               node_id);
  EXPECT_TRUE(entry.good());
  entry.Put(syncable::BASE_VERSION, 1);
  entry.Put(syncable::SERVER_VERSION, 1);
  entry.Put(syncable::IS_UNAPPLIED_UPDATE, false);
  entry.Put(syncable::SERVER_PARENT_ID, root_id);
  entry.Put(syncable::PARENT_ID, root_id);
  entry.Put(syncable::SERVER_IS_DIR, false);
  entry.Put(syncable::IS_DIR, false);
  entry.Put(syncable::SERVER_SPECIFICS, specifics);
  entry.Put(syncable::NON_UNIQUE_NAME, client_tag);
  entry.Put(syncable::UNIQUE_CLIENT_TAG, hashed_tag);
  entry.Put(syncable::IS_DEL, false);
  entry.Put(syncable::SPECIFICS, specifics);
  return entry.Get(syncable::META_HANDLE);
}
