  void StartTimeout() {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    if (!service_) {
      return;
    }
    timeout_start_time_ = base::TimeTicks::Now();
    BrowserThread::PostDelayedTask(
        BrowserThread::UI,
        FROM_HERE,
        base::Bind(&CheckClientDownloadRequest::Cancel,
                   weakptr_factory_.GetWeakPtr()),
        base::TimeDelta::FromMilliseconds(
            service_->download_request_timeout_ms()));
  }
