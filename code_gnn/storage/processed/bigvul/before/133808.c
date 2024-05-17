  void FinishRequest(DownloadCheckResult result,
                     DownloadCheckResultReason reason) {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    if (finished_) {
      return;
    }
    finished_ = true;
    weakptr_factory_.InvalidateWeakPtrs();
    if (!request_start_time_.is_null()) {
      UMA_HISTOGRAM_ENUMERATION("SBClientDownload.DownloadRequestNetworkStats",
                                reason,
                                REASON_MAX);
    }
    if (!timeout_start_time_.is_null()) {
      UMA_HISTOGRAM_ENUMERATION("SBClientDownload.DownloadRequestTimeoutStats",
                                reason,
                                REASON_MAX);
      if (reason != REASON_REQUEST_CANCELED) {
        UMA_HISTOGRAM_TIMES("SBClientDownload.DownloadRequestTimeoutDuration",
                            base::TimeTicks::Now() - timeout_start_time_);
      }
    }
    if (service_) {
      DVLOG(2) << "SafeBrowsing download verdict for: "
               << item_->DebugString(true) << " verdict:" << reason
               << " result:" << result;
      UMA_HISTOGRAM_ENUMERATION("SBClientDownload.CheckDownloadStats",
                                reason,
                                REASON_MAX);
      callback_.Run(result);
      item_->RemoveObserver(this);
      item_ = NULL;
      DownloadProtectionService* service = service_;
      service_ = NULL;
      service->RequestFinished(this);
    } else {
      callback_.Run(UNKNOWN);
    }
  }
