void QuotaManager::DeleteOriginFromDatabase(
    const GURL& origin, StorageType type) {
  LazyInitialize();
  if (db_disabled_)
    return;

  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&DeleteOriginInfoOnDBThread, origin, type),
      base::Bind(&QuotaManager::DidDatabaseWork,
                 weak_factory_.GetWeakPtr()));
}
