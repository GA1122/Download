void CallQuotaCallback(
    const QuotaCallback& callback,
    StorageType type,
    QuotaStatusCode status,
    const QuotaAndUsage& quota_and_usage) {
  callback.Run(status, type, quota_and_usage.quota);
}
