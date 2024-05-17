void DownloadItemImpl::MaybeCompleteDownload() {
  delegate_->MaybeCompleteDownload(this);
}
