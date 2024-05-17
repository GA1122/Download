  GlobalUsageCallback NewWaitableGlobalUsageCallback() {
    ++waiting_callbacks_;
    return base::Bind(&UsageAndQuotaDispatcherTask::DidGetGlobalUsage,
                      weak_factory_.GetWeakPtr());
  }
