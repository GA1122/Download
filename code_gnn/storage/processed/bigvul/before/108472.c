QuotaManager::UsageAndQuotaDispatcherTask::Create(
    QuotaManager* manager, bool global,
    const QuotaManager::HostAndType& host_and_type) {
  if (global)
    return new UsageAndQuotaDispatcherTaskForTemporaryGlobal(
        manager, host_and_type);
  switch (host_and_type.second) {
    case kStorageTypeTemporary:
      return new UsageAndQuotaDispatcherTaskForTemporary(
          manager, host_and_type);
    case kStorageTypePersistent:
      return new UsageAndQuotaDispatcherTaskForPersistent(
          manager, host_and_type);
    default:
      NOTREACHED();
  }
  return NULL;
}
