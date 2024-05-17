void UrlFetcherDownloader::OnDownloadProgress(int64_t current) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  CrxDownloader::OnDownloadProgress();
}
