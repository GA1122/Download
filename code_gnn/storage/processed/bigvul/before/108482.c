  void DidGetGlobalUsage(StorageType type, int64 usage, int64 unlimited_usage) {
    DCHECK_EQ(this->type(), type);
    DCHECK_GE(usage, unlimited_usage);
    if (quota_status_ == kQuotaStatusUnknown)
      quota_status_ = kQuotaStatusOk;
    global_usage_ = usage;
    global_unlimited_usage_ = unlimited_usage;
    CheckCompleted();
  }
