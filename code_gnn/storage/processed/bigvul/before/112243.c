void UrlFetcher::Core::CancelRequest() {
  if (request_.get()) {
    request_->Cancel();
    request_.reset();
  }
}
