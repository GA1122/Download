void QuotaManager::NotifyStorageModifiedInternal(
    QuotaClient::ID client_id,
    const GURL& origin,
    StorageType type,
    int64 delta,
    base::Time modified_time) {
  LazyInitialize();
  GetUsageTracker(type)->UpdateUsageCache(client_id, origin, delta);

  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&UpdateModifiedTimeOnDBThread, origin, type, modified_time),
      base::Bind(&QuotaManager::DidDatabaseWork,
                 weak_factory_.GetWeakPtr()));
}
