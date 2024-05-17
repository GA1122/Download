void DownloadManagerImpl::Shutdown() {
  DVLOG(20) << __func__ << "() shutdown_needed_ = " << shutdown_needed_;
  if (!shutdown_needed_)
    return;
  shutdown_needed_ = false;

  for (auto& observer : observers_)
    observer.ManagerGoingDown(this);

  for (const auto& it : downloads_) {
    download::DownloadItemImpl* download = it.second.get();
    if (download->GetState() == download::DownloadItem::IN_PROGRESS)
      download->Cancel(false);
  }
  downloads_.clear();
  downloads_by_guid_.clear();
  url_download_handlers_.clear();

  observers_.Clear();

  in_progress_manager_->ShutDown();

  if (delegate_)
    delegate_->Shutdown();
  delegate_ = nullptr;
}
