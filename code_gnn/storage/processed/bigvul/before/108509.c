void QuotaManager::GetUsageAndQuota(
    const GURL& origin, StorageType type,
    const GetUsageAndQuotaCallback& callback) {
  GetUsageAndQuotaInternal(
      origin, type, false  ,
      base::Bind(&CallGetUsageAndQuotaCallback, callback,
                 IsStorageUnlimited(origin), IsInstalledApp(origin)));
}
