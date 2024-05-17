  AvailableSpaceCallback NewWaitableAvailableSpaceCallback() {
    ++waiting_callbacks_;
    return base::Bind(&UsageAndQuotaDispatcherTask::DidGetAvailableSpace,
                      weak_factory_.GetWeakPtr());
  }
