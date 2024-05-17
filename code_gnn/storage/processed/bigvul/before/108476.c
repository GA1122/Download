bool DeleteOriginInfoOnDBThread(const GURL& origin,
                                StorageType type,
                                QuotaDatabase* database) {
  DCHECK(database);
  return database->DeleteOriginInfo(origin, type);
}
