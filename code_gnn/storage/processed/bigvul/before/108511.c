void QuotaManager::GetUsageAndQuotaInternal(
    const GURL& origin, StorageType type, bool global,
    const UsageAndQuotaDispatcherCallback& callback) {
  LazyInitialize();

  StorageType requested_type = type;
  if (type == kStorageTypeUnknown) {
    callback.Run(kQuotaErrorNotSupported, QuotaAndUsage());
    return;
  }

  const int kStorageTypeTemporaryGlobal = kStorageTypeTemporary + 100;
  if (global) {
    DCHECK_EQ(kStorageTypeTemporary, type);
    type = static_cast<StorageType>(kStorageTypeTemporaryGlobal);
  }

  std::string host = net::GetHostOrSpecFromURL(origin);
  HostAndType host_and_type = std::make_pair(host, type);
  UsageAndQuotaDispatcherTaskMap::iterator found =
      usage_and_quota_dispatchers_.find(host_and_type);
  if (found == usage_and_quota_dispatchers_.end()) {
    UsageAndQuotaDispatcherTask* dispatcher =
        UsageAndQuotaDispatcherTask::Create(this, global, host_and_type);
    found = usage_and_quota_dispatchers_.insert(
        std::make_pair(host_and_type, dispatcher)).first;
  }
  if (found->second->AddCallback(callback) &&
      (requested_type != kStorageTypeTemporary ||
       temporary_quota_initialized_)) {
    found->second->Start();
  }
}
