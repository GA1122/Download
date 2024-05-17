DownloadItem::DownloadState DownloadItemImpl::GetState() const {
  return InternalToExternalState(state_);
}
