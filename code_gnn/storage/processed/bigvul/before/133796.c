  void Cancel() {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    if (fetcher_.get()) {
      fetcher_.reset();
    }
    FinishRequest(UNKNOWN, REASON_REQUEST_CANCELED);
  }
