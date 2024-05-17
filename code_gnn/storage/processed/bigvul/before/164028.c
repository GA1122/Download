void DownloadManagerImpl::DropDownload() {
  download::RecordDownloadCount(download::DOWNLOAD_DROPPED_COUNT);
  for (auto& observer : observers_)
    observer.OnDownloadDropped(this);
}
