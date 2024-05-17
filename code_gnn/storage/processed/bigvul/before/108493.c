void QuotaManager::DidSetPersistentHostQuota(const std::string& host,
                                             const HostQuotaCallback& callback,
                                             const int64* new_quota,
                                             bool success) {
  DidDatabaseWork(success);
  callback.Run(success ? kQuotaStatusOk : kQuotaErrorInvalidAccess,
               host, kStorageTypePersistent, *new_quota);
}
