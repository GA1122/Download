  HostUsageCallback NewWaitableHostUsageCallback() {
    ++waiting_callbacks_;
    return base::Bind(&UsageAndQuotaDispatcherTask::DidGetHostUsage,
                      weak_factory_.GetWeakPtr());
  }
