  void PostFinishTask(DownloadCheckResult result,
                      DownloadCheckResultReason reason) {
    BrowserThread::PostTask(
        BrowserThread::UI,
        FROM_HERE,
        base::Bind(&CheckClientDownloadRequest::FinishRequest, this, result,
                   reason));
  }
