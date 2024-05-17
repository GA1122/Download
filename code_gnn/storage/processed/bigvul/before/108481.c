  void DidGetAvailableSpace(QuotaStatusCode status, int64 space) {
    DCHECK_GE(space, 0);
    if (quota_status_ == kQuotaStatusUnknown || quota_status_ == kQuotaStatusOk)
      quota_status_ = status;
    available_space_ = space;
    CheckCompleted();
  }
