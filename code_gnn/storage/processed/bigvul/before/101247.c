  sync_pb::EntitySpecifics DefaultPreferencesSpecifics() {
    sync_pb::EntitySpecifics result;
    AddDefaultExtensionValue(syncable::PREFERENCES, &result);
    return result;
  }
