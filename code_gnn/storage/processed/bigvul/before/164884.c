void DownloadManagerImpl::OnDownloadCreated(
    std::unique_ptr<download::DownloadItemImpl> download) {
  DCHECK(!base::ContainsKey(downloads_, download->GetId()));
  DCHECK(!base::ContainsKey(downloads_by_guid_, download->GetGuid()));
  download::DownloadItemImpl* item = download.get();
  downloads_[item->GetId()] = std::move(download);
  downloads_by_guid_[item->GetGuid()] = item;
  for (auto& observer : observers_)
    observer.OnDownloadCreated(this, item);
  DVLOG(20) << __func__ << "() download = " << item->DebugString(true);
}
