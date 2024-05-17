  sync_pb::EntitySpecifics DefaultBookmarkSpecifics() {
    sync_pb::EntitySpecifics result;
    AddDefaultExtensionValue(syncable::BOOKMARKS, &result);
    return result;
  }
