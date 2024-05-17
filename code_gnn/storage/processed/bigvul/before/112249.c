void UrlFetcher::Core::ReadResponse() {
  int bytes_read = 0;
  if (request_->status().is_success()) {
    request_->Read(buffer_, kBufferSize, &bytes_read);
  }
  OnReadCompleted(request_.get(), bytes_read);
}
