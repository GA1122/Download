bool AppCacheUpdateJob::URLFetcher::MaybeRetryRequest() {
  if (retry_503_attempts_ >= kMax503Retries ||
      !request_->response_headers()->HasHeaderValue("retry-after", "0")) {
    return false;
  }
  ++retry_503_attempts_;
  result_ = UPDATE_OK;
  request_ = job_->service_->request_context()->CreateRequest(
      url_, net::DEFAULT_PRIORITY, this);
  Start();
  return true;
}
