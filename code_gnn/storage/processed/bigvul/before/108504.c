   bool GetModifiedSinceOnDBThread(StorageType type,
                                  base::Time modified_since,
                                  QuotaDatabase* database) {
    DCHECK(database);
    return database->GetOriginsModifiedSince(type, &origins_, modified_since);
  }
