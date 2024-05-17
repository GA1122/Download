  sync_pb::EntitySpecifics DefaultPreferencesSpecifics() {
    sync_pb::EntitySpecifics result;
    AddDefaultFieldValue(syncable::PREFERENCES, &result);
    return result;
  }
