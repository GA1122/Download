void DownloadItemImpl::OnDownloadedFileRemoved() {
  file_externally_removed_ = true;
  DVLOG(20) << __func__ << "() download=" << DebugString(true);
  UpdateObservers();
}
