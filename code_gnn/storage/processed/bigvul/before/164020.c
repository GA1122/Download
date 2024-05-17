void CreateInterruptedDownload(
    std::unique_ptr<download::DownloadUrlParameters> params,
    download::DownloadInterruptReason reason,
    base::WeakPtr<DownloadManagerImpl> download_manager) {
  std::unique_ptr<download::DownloadCreateInfo> failed_created_info(
      new download::DownloadCreateInfo(
          base::Time::Now(), base::WrapUnique(new download::DownloadSaveInfo)));
  failed_created_info->url_chain.push_back(params->url());
  failed_created_info->result = reason;
  std::unique_ptr<ByteStreamReader> empty_byte_stream;
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(
          &DownloadManager::StartDownload, download_manager,
          std::move(failed_created_info),
          std::make_unique<ByteStreamInputStream>(std::move(empty_byte_stream)),
          nullptr, params->callback()));
}
