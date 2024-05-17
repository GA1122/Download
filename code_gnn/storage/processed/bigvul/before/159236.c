void CreateInterruptedDownload(
    DownloadUrlParameters* params,
    DownloadInterruptReason reason,
    base::WeakPtr<DownloadManagerImpl> download_manager) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::unique_ptr<DownloadCreateInfo> failed_created_info(
      new DownloadCreateInfo(base::Time::Now(),
                             base::WrapUnique(new DownloadSaveInfo)));
  failed_created_info->url_chain.push_back(params->url());
  failed_created_info->result = reason;
  std::unique_ptr<ByteStreamReader> empty_byte_stream;
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(&DownloadManager::StartDownload, download_manager,
                     std::move(failed_created_info),
                     std::make_unique<DownloadManager::InputStream>(
                         std::move(empty_byte_stream)),
                     params->callback()));
}
