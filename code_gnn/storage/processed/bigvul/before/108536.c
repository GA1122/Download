bool QuotaManager::ResetUsageTracker(StorageType type) {
  DCHECK(GetUsageTracker(type));
  if (GetUsageTracker(type)->IsWorking())
    return false;
  switch (type) {
    case kStorageTypeTemporary:
      temporary_usage_tracker_.reset(
          new UsageTracker(clients_, kStorageTypeTemporary,
                           special_storage_policy_));
      return true;
    case kStorageTypePersistent:
      persistent_usage_tracker_.reset(
          new UsageTracker(clients_, kStorageTypePersistent,
                           special_storage_policy_));
      return true;
    default:
      NOTREACHED();
  }
  return true;
}
