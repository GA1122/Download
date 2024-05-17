  HostQuotaCallback NewWaitableHostQuotaCallback() {
    ++waiting_callbacks_;
    return base::Bind(&UsageAndQuotaDispatcherTask::DidGetHostQuota,
                      weak_factory_.GetWeakPtr());
  }
