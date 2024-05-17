void QuotaManager::DidGetPersistentHostQuota(const HostQuotaCallback& callback,
                                             const std::string& host,
                                             const int64* quota,
                                             bool success) {
  DidDatabaseWork(success);
  callback.Run(kQuotaStatusOk, host, kStorageTypePersistent, *quota);
}
