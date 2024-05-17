void AppCacheUpdateJob::URLFetcher::ReadResponseData() {
  InternalUpdateState state = job_->internal_state_;
  if (state == CACHE_FAILURE || state == CANCELLED || state == COMPLETED)
    return;
  int bytes_read = 0;
  request_->Read(buffer_.get(), kBufferSize, &bytes_read);
  OnReadCompleted(request_.get(), bytes_read);
}
