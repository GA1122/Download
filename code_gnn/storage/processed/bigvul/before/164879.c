download::DownloadItem* DownloadManagerImpl::CreateDownloadItem(
    const std::string& guid,
    uint32_t id,
    const base::FilePath& current_path,
    const base::FilePath& target_path,
    const std::vector<GURL>& url_chain,
    const GURL& referrer_url,
    const GURL& site_url,
    const GURL& tab_url,
    const GURL& tab_refererr_url,
    const std::string& mime_type,
    const std::string& original_mime_type,
    base::Time start_time,
    base::Time end_time,
    const std::string& etag,
    const std::string& last_modified,
    int64_t received_bytes,
    int64_t total_bytes,
    const std::string& hash,
    download::DownloadItem::DownloadState state,
    download::DownloadDangerType danger_type,
    download::DownloadInterruptReason interrupt_reason,
    bool opened,
    base::Time last_access_time,
    bool transient,
    const std::vector<download::DownloadItem::ReceivedSlice>& received_slices) {
  auto in_progress_download = RetrieveInProgressDownload(id);
#if defined(OS_ANDROID)
  if (cleared_download_guids_on_startup_.find(guid) !=
      cleared_download_guids_on_startup_.end()) {
    return nullptr;
  }
  if (url_chain.empty() ||
      ShouldClearDownloadFromDB(url_chain.back(), state, interrupt_reason)) {
    DeleteDownloadedFileOnUIThread(current_path);
    return nullptr;
  }
#endif
  auto item = base::WrapUnique(item_factory_->CreatePersistedItem(
      this, guid, id, current_path, target_path, url_chain, referrer_url,
      site_url, tab_url, tab_refererr_url, mime_type, original_mime_type,
      start_time, end_time, etag, last_modified, received_bytes, total_bytes,
      hash, state, danger_type, interrupt_reason, opened, last_access_time,
      transient, received_slices));
  if (in_progress_download) {
    if (item->IsDone()) {
      in_progress_manager_->RemoveInProgressDownload(guid);
    } else {
      item = std::move(in_progress_download);
      item->SetDelegate(this);
    }
  }
  download::DownloadItemImpl* download = item.get();
  DownloadItemUtils::AttachInfo(download, GetBrowserContext(), nullptr);
  OnDownloadCreated(std::move(item));
  return download;
}
