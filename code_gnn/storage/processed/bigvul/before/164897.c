void DownloadResourceHandler::OnReadyToRead() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  Resume();
}
