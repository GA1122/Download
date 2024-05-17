  void DidGetHostUsage(const std::string& host, StorageType type, int64 usage) {
    DCHECK_EQ(this->host(), host);
    DCHECK_EQ(this->type(), type);
    if (quota_status_ == kQuotaStatusUnknown)
      quota_status_ = kQuotaStatusOk;
    host_usage_ = usage;
    CheckCompleted();
  }
