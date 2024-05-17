  int64 CreateUnsyncedDirectory(const string& entry_name,
      const string& idstring) {
    return CreateUnsyncedDirectory(entry_name,
        syncable::Id::CreateFromServerId(idstring));
  }
