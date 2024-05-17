void QuotaManager::ReportHistogram() {
  GetGlobalUsage(kStorageTypeTemporary,
                 base::Bind(
                     &QuotaManager::DidGetTemporaryGlobalUsageForHistogram,
                     weak_factory_.GetWeakPtr()));
  GetGlobalUsage(kStorageTypePersistent,
                 base::Bind(
                     &QuotaManager::DidGetPersistentGlobalUsageForHistogram,
                     weak_factory_.GetWeakPtr()));
}
