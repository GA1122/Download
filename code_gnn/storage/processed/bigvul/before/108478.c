  void DidDeleteOriginData(QuotaStatusCode status) {
    DCHECK_GT(remaining_deleters_, 0);

    if (status != kQuotaStatusOk)
      ++error_count_;

    if (--remaining_deleters_ == 0)
      CallCompleted();
  }
