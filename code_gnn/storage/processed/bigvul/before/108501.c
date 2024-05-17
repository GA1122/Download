void QuotaManager::GetHostUsage(const std::string& host,
                                StorageType type,
                                const HostUsageCallback& callback) {
  LazyInitialize();
  GetUsageTracker(type)->GetHostUsage(host, callback);
}
