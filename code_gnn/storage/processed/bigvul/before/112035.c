  sync_pb::EntitySpecifics DefaultBookmarkSpecifics() {
    sync_pb::EntitySpecifics result;
    AddDefaultFieldValue(syncable::BOOKMARKS, &result);
    return result;
  }
