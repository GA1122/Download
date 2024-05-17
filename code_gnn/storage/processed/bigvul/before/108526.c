void QuotaManager::NotifyStorageAccessedInternal(
    QuotaClient::ID client_id,
    const GURL& origin, StorageType type,
    base::Time accessed_time) {
  LazyInitialize();
  if (type == kStorageTypeTemporary && !lru_origin_callback_.is_null()) {
    access_notified_origins_.insert(origin);
  }

  if (db_disabled_)
    return;
  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&UpdateAccessTimeOnDBThread, origin, type, accessed_time),
      base::Bind(&QuotaManager::DidDatabaseWork,
                 weak_factory_.GetWeakPtr()));
}
