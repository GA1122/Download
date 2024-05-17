const sync_pb::EntitySpecifics& BaseNode::GetUnencryptedSpecifics(
    const syncable::Entry* entry) const {
  const sync_pb::EntitySpecifics& specifics = entry->Get(SPECIFICS);
  if (specifics.has_encrypted()) {
    DCHECK_NE(syncable::GetModelTypeFromSpecifics(unencrypted_data_),
              syncable::UNSPECIFIED);
    return unencrypted_data_;
  } else {
    if (GetModelType() == syncable::BOOKMARKS) {
      const sync_pb::BookmarkSpecifics& bookmark_specifics =
          specifics.GetExtension(sync_pb::bookmark);
      if (bookmark_specifics.has_title() ||
          GetTitle().empty() ||   
          !GetEntry()->Get(syncable::UNIQUE_SERVER_TAG).empty()) {
        return specifics;
      } else {
        DCHECK_EQ(syncable::GetModelTypeFromSpecifics(unencrypted_data_),
                  syncable::BOOKMARKS);
        return unencrypted_data_;
      }
    } else {
      DCHECK_EQ(syncable::GetModelTypeFromSpecifics(unencrypted_data_),
                syncable::UNSPECIFIED);
      return specifics;
    }
  }
}
