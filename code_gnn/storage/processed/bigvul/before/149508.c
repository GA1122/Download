void UrlFetcherDownloader::OnResponseStarted(int response_code,
                                             int64_t content_length) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  VLOG(1) << "url fetcher response started for: " << url().spec();

  response_code_ = response_code;
  total_bytes_ = content_length;
}
