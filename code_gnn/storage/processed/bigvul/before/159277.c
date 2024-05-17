void DownloadManagerImpl::StartDownload(
    std::unique_ptr<DownloadCreateInfo> info,
    std::unique_ptr<DownloadManager::InputStream> stream,
    const DownloadUrlParameters::OnStartedCallback& on_started) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(info);

  DCHECK(
      (info->result == DOWNLOAD_INTERRUPT_REASON_NONE && !stream->IsEmpty()) ||
      (info->result != DOWNLOAD_INTERRUPT_REASON_NONE && stream->IsEmpty()));
  DVLOG(20) << __func__
            << "() result=" << DownloadInterruptReasonToString(info->result);
  uint32_t download_id = info->download_id;
  const bool new_download = (download_id == content::DownloadItem::kInvalidId);
  if (new_download)
    RecordDownloadConnectionSecurity(info->url(), info->url_chain);
  base::Callback<void(uint32_t)> got_id(base::Bind(
      &DownloadManagerImpl::StartDownloadWithId, weak_factory_.GetWeakPtr(),
      base::Passed(&info), base::Passed(&stream), on_started, new_download));
  if (new_download) {
    GetNextId(got_id);
  } else {
    got_id.Run(download_id);
  }
}
