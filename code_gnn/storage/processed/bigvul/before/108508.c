 void QuotaManager::GetTemporaryGlobalQuota(const QuotaCallback& callback) {
  if (temporary_quota_override_ > 0) {
    callback.Run(kQuotaStatusOk, kStorageTypeTemporary,
                 temporary_quota_override_);
    return;
  }
  GetUsageAndQuotaInternal(
      GURL(), kStorageTypeTemporary, true  ,
      base::Bind(&CallQuotaCallback, callback, kStorageTypeTemporary));
}
