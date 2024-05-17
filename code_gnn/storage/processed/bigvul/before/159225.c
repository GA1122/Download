void DownloadManagerImpl::AddUrlDownloadHandler(
    UniqueUrlDownloadHandlerPtr downloader) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (downloader)
    url_download_handlers_.push_back(std::move(downloader));
}
