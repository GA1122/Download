void QuotaManager::GetGlobalUsage(StorageType type,
                                  const GlobalUsageCallback& callback) {
  LazyInitialize();
  GetUsageTracker(type)->GetGlobalUsage(callback);
}
