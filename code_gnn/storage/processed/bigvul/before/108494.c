void QuotaManager::DidSetTemporaryGlobalOverrideQuota(
    const QuotaCallback& callback,
    const int64* new_quota,
    bool success) {
  QuotaStatusCode status = kQuotaErrorInvalidAccess;
  DidDatabaseWork(success);
  if (success) {
    temporary_quota_override_ = *new_quota;
    status = kQuotaStatusOk;
  }

  if (callback.is_null())
    return;

  callback.Run(status, kStorageTypeTemporary, *new_quota);
}
