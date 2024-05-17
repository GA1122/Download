  void CallCallbacksAndClear(
      QuotaStatusCode status,
      int64 usage, int64 unlimited_usage, int64 quota,
      int64 available_space) {
    QuotaAndUsage qau = { usage, unlimited_usage, quota, available_space };
    for (CallbackList::iterator iter = callbacks_.begin();
         iter != callbacks_.end(); ++iter) {
      (*iter).Run(status, qau);
    }
    callbacks_.clear();
  }
