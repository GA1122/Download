void DownloadManagerImpl::ReportBytesWasted(
    download::DownloadItemImpl* download) {
  in_progress_manager_->ReportBytesWasted(download);
}
