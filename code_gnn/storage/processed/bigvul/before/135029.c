void  AppCacheUpdateJob::URLFetcher::OnWriteComplete(int result) {
  if (result < 0) {
    request_->Cancel();
    result_ = DISKCACHE_ERROR;
    OnResponseCompleted();
    return;
  }
  ReadResponseData();
}
