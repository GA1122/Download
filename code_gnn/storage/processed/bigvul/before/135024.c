void AppCacheUpdateJob::URLFetcher::OnReadCompleted(
    net::URLRequest* request, int bytes_read) {
  DCHECK_EQ(request_.get(), request);
  bool data_consumed = true;
  if (request->status().is_success() && bytes_read > 0) {
    job_->MadeProgress();
    data_consumed = ConsumeResponseData(bytes_read);
    if (data_consumed) {
      bytes_read = 0;
      while (request->Read(buffer_.get(), kBufferSize, &bytes_read)) {
        if (bytes_read > 0) {
          data_consumed = ConsumeResponseData(bytes_read);
          if (!data_consumed)
            break;   
        } else {
          break;
        }
      }
    }
  }
  if (data_consumed && !request->status().is_io_pending()) {
    DCHECK_EQ(UPDATE_OK, result_);
    OnResponseCompleted();
  }
}
