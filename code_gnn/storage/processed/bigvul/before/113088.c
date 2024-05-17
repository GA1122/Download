void DownloadItemImpl::OnDownloadedFileRemoved() {
  file_externally_removed_ = true;
  UpdateObservers();
}
