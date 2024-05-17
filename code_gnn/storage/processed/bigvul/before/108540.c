void QuotaManager::SetTemporaryGlobalOverrideQuota(
    int64 new_quota, const QuotaCallback& callback) {
  LazyInitialize();

  if (new_quota < 0) {
    if (!callback.is_null())
      callback.Run(kQuotaErrorInvalidModification,
                   kStorageTypeTemporary, -1);
    return;
  }

  if (db_disabled_) {
    if (callback.is_null())
      callback.Run(kQuotaErrorInvalidAccess,
                   kStorageTypeTemporary, -1);
    return;
  }

  int64* new_quota_ptr = new int64(new_quota);
  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&SetTemporaryGlobalOverrideQuotaOnDBThread,
                 base::Unretained(new_quota_ptr)),
      base::Bind(&QuotaManager::DidSetTemporaryGlobalOverrideQuota,
                 weak_factory_.GetWeakPtr(),
                 callback,
                 base::Owned(new_quota_ptr)));
}
