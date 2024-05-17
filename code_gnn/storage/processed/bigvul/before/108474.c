void QuotaManager::DeleteOriginData(
    const GURL& origin, StorageType type, int quota_client_mask,
    const StatusCallback& callback) {
  LazyInitialize();

  if (origin.is_empty() || clients_.empty()) {
    callback.Run(kQuotaStatusOk);
    return;
  }

  OriginDataDeleter* deleter =
      new OriginDataDeleter(this, origin, type, quota_client_mask, callback);
  deleter->Start();
}
