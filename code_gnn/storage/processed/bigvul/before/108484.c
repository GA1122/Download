  void DidGetHostQuota(QuotaStatusCode status,
                       const std::string& host,
                       StorageType type,
                       int64 host_quota) {
    DCHECK_EQ(this->host(), host);
    DCHECK_EQ(this->type(), type);
    if (quota_status_ == kQuotaStatusUnknown || quota_status_ == kQuotaStatusOk)
      quota_status_ = status;
    host_quota_ = host_quota;
    CheckCompleted();
  }
